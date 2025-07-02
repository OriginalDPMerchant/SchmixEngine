{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE RecordWildCards #-}
{-# LANGUAGE InstanceSigs #-}

module Types where

import Text.Megaparsec
import Data.Bits
import Data.Void
import Data.Proxy
import Data.List.NonEmpty(NonEmpty)
import qualified Data.List.NonEmpty as NonEmpty

type Lexer = Parsec Void String
type Parser = Parsec Void [CppToken]

data CppToken
    = Preprocessor
    | Whitespace
    | Comment String
    | Keyword String
    | Punctuator Char
    | Tag String
    | Identifier String
    | Digit Int
    deriving(Show, Eq, Ord)

class CppAstNode a where
    makeHeader :: [a] -> a
    makeNamespace :: String -> [a] -> a
    makeClass :: String -> String -> [a] -> a
    makeEnum :: String -> String -> [a] -> a
    generateCode :: a -> String

instance VisualStream [CppToken] where
  showTokens :: Proxy [CppToken] -> NonEmpty (Token [CppToken]) -> String
  showTokens Proxy y = show (NonEmpty.head y)
  tokensLength :: Proxy [CppToken] -> NonEmpty (Token [CppToken]) -> Int
  tokensLength Proxy = length