module Parser where

import Constants
import Types

import Text.Megaparsec
import Data.Void
import Data.Char
import Data.List
import Data.Maybe
import Util
import Data.Functor
import Config
import Data.List

import Data.Set (Set)
import qualified Data.Set as Set

isIdentifier :: CppToken -> Bool
isIdentifier (Identifier _) = True
isIdentifier _ = False

namespaceKeyword :: Parser CppToken
namespaceKeyword = single (Keyword "namespace") <?> "namespace"

enumKeyword :: Parser CppToken
enumKeyword = single (Keyword "enum") <?> "enum"

classKeyword :: Parser CppToken
classKeyword = (single (Keyword "class") <?> "class") <|> (single (Keyword "struct") <?> "struct")

accessModifier :: Parser CppToken
accessModifier = (single (Keyword "public") <|> single (Keyword "private") <|> single (Keyword "protected")) <?> "access modifier"

identifier :: Parser String
identifier = do
    Identifier x <- satisfy isIdentifier <?> "identifier"
    return x

qualifiedName :: Parser String
qualifiedName = do
    x <- identifier
    xs <- many helper
    return $ concat (x:xs)
    where helper = do
            _ <- chunk [Punctuator ':', Punctuator ':']
            xs <- identifier
            return (':':':':xs)

parseClassParent :: Parser String
parseClassParent = do
    _ <- single (Punctuator ':')
    _ <- optional accessModifier
    qualifiedName

parseNamespace :: CppAstNode a => String -> ParserGraph a ->  Parser a
parseNamespace name graph = do
    _ <- namespaceKeyword
    namespace <- qualifiedName
    _ <- single (Punctuator '{')
    children <- getParsers (InjectAt name) graph >>|* single (Punctuator '}')
    _ <- single (Punctuator '}')
    return $ makeNamespace namespace children

parseClass :: CppAstNode a => String -> ParserGraph a ->  Parser a
parseClass name graph = do
    _ <- classKeyword
    class' <- identifier
    _ <- optional $ single (Identifier "final")
    base <- optional parseClassParent
    _ <- single (Punctuator '{')
    children <- getParsers (InjectAt name) graph >>|* single (Punctuator '}')
    _ <- single (Punctuator '}')
    _ <- single (Punctuator ';')
    return $ makeClass class' (fromMaybe "" base) children

parseEnum :: CppAstNode a => String -> ParserGraph a ->  Parser a
parseEnum name graph = do
    _ <- enumKeyword
    _ <- optional classKeyword
    name' <- identifier
    base <- optional parseClassParent
    _ <- single (Punctuator '{')
    members <- getParsers (InjectAt name) graph >>|* single (Punctuator '}')--sepEndBy parseEnumMember (single (Punctuator ','))
    _ <- single (Punctuator '}')
    _ <- single (Punctuator ';')
    return $ makeEnum name' (fromMaybe "" base) members 

parseHeader :: CppAstNode a => ParserGraph a -> Parser a
parseHeader graph = (getParsers Global graph >>|* eof) <&> makeHeader

buildAst :: CppAstNode a => ParserGraph a -> [CppToken] -> String -> Either (ParseErrorBundle [CppToken] Void) a
buildAst config xs fileName = parse (parseHeader config) fileName xs