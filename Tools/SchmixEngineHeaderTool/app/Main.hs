module Main (main) where
import Lexer
import Parser
import Text.Megaparsec
import Types
import Text.Megaparsec (errorBundlePretty)
import Data.Foldable (Foldable(toList))
import Config
import Data.Set (Set)
import qualified Data.Set as Set

data SchmixEngineAstNode = Header [SchmixEngineAstNode]
                         | Namespace String [SchmixEngineAstNode]
                         | Class String String [SchmixEngineAstNode]
                         | ClassTag [SchmixEngineAstNode]
                         | Enum String String [SchmixEngineAstNode]
                         | EnumTag [SchmixEngineAstNode]
                         | EnumMember String
                         | Property String
    deriving Show

instance CppAstNode SchmixEngineAstNode where
    makeHeader = Header
    makeNamespace = Namespace
    makeClass = Class
    makeEnum = Enum

parseEnumMember :: String -> ParserGraph SchmixEngineAstNode ->  Parser SchmixEngineAstNode
parseEnumMember _ _ = do
    x <- identifier
    _ <- manyTill anySingle (lookAhead (satisfy (\y -> y == Punctuator ',' || y == Punctuator '}')))
    return $ EnumMember x

config :: Config SchmixEngineAstNode
config = Config
    {
        tags = Set.fromList ["SMX_CLASS", "SMX_ENUM", "SMX_ENUM_DEFAULT", "SMX_PROPERTY", "SMX_SERIALIZE", "SMX_GETTER", "SMX_SETTER", "SMX_READ_ONLY", "SMX_EXPOSE_TO_LUA"],

        parsers = Set.fromList
            [
                ParserConfig
                {
                    parserName = "Namespace",
                    parser = parseNamespace,
                    parserInjectionPoints = Set.fromList [Global, InjectAt "Namespace"]
                },
                ParserConfig
                {
                    parserName = "Class",
                    parser = parseClass,
                    parserInjectionPoints = Set.fromList [Global, InjectAt "Namespace", InjectAt "Class"]
                },
                ParserConfig
                {
                    parserName = "Enum",
                    parser = parseEnum,
                    parserInjectionPoints = Set.fromList [Global, InjectAt "Namespace", InjectAt "Class"]
                },
                ParserConfig
                {
                    parserName = "Enum",
                    parser = parseEnum,
                    parserInjectionPoints = Set.fromList [Global, InjectAt "Namespace", InjectAt "Class"]
                },
                ParserConfig
                {
                    parserName = "EnumMember",
                    parser = parseEnumMember,
                    parserInjectionPoints = Set.fromList [InjectAt "Enum"]
                }
            ]
    }

main :: IO ()
main = do
    let fileName = "C:/Users/feran/Documents/Programming Projects/SchmixEngine/Engine/Source/SchmixEngine/Core/Graphics/Core/Buffer.h"
    headerFile <- readFile fileName
    let tokens = lexicalAnalysis (tags config) headerFile fileName
    -- either (putStrLn . errorBundlePretty) print tokens
    case tokens of
        (Left err) -> putStrLn (errorBundlePretty err)
        (Right xs) -> case buildAst (makeParserGraph (parsers config)) xs fileName of
                            (Left err) -> putStrLn (parseErrorPretty (head $ toList (bundleErrors err)))
                            (Right x)  -> print x--putStrLn (unlines (map genReflection (filterAst $ flattenAst x)))
    return ()
    --let x = tokenizeHeader TokenizerState { text = mergeWhiteSspliceLines headerFile, tokens = []}
    --putStr (text x)
    --putStrLn (show $ length $ tokens x)
    --return x
