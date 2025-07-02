module CodeGen where

import Types
 
{-
flattenAst :: CppAstNode -> [CppAstNode]

flattenAst (Header xs) = concatMap flattenAst xs
flattenAst (Namespace name xs) = concatMap (flattenAst . helper) xs
    where helper (Namespace y ys) = Namespace (concat [name, "::", y]) ys
          helper (Class x) = Class x { className = concat [name, "::", className x] }
          helper (Enum x) = Enum x { enumName = concat [name, "::", enumName x] }
          helper x = x

flattenAst (Class x) = Class x : concatMap (flattenAst . helper) (classMembers x)
    where helper (Namespace y ys) = Namespace (concat [className x, "::", y]) ys
          helper (Class y) = Class y { className = concat [className x, "::", className y] }
          helper (Enum y) = Enum y { enumName = concat [className x, "::", enumName y] }
          helper y = y

flattenAst x = [x]

filterAst :: [CppAstNode] -> [CppAstNode]
filterAst = filter helper
    where helper (Class x) = isClassReflected x
          helper (Enum x) = isEnumReflected x
          helper _ = False

genReflection :: CppAstNode -> [Char]
genReflection (Class x) = concat 
    [
        "template<> inline bool ClassDB_CT::HasClassInfo<", className x , ">() { return true; }\n",

        "template<> inline bool ClassDB_CT::InitClassInfo<", className x, ">()\n",
        "{\n",
	    "\tReflectionUtil::RegisterClassName<", className x, ">(\"", className x, "\");\n",
	    "\treturn true;\n",
        "}\n"
    ]

genReflection (Enum x) = concat 
    [
        "template<> inline bool SchmixEngine::ClassDB_CT::HasClassInfo<", enumName x , ">() { return true; }\n",

        "template<> inline bool ClassDB_CT::InitClassInfo<", enumName x, ">()\n",
        "{\n",
	    "\tReflectionUtil::RegisterClassName<", enumName x, ">(\"", enumName x, "\");\n",
        if null $ enumBaseType x then "" else concat ["\tReflectionUtil::RegisterParentClass<", enumName x, " , ", enumBaseType x, ">();\n"],
	    "\treturn true;\n",
        "}\n"
    ]
genReflection _ = ""-}