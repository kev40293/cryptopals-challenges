module Cryptopals.EncXor
(
xorBuffers,
encryptByte,
encryptMult,
encryptRepByte,
scoreGuess
) where

import Data.Bits
import Data.ByteString
import qualified Data.ByteString.Char8 as C
import Data.Word
import Data.Char
import Data.List

xorBuffers [] [] = []
xorBuffers (x:xs) (y:ys)
   | Prelude.length xs == Prelude.length ys = (xor x y):(xorBuffers xs ys)
   | otherwise = []

encryptByte :: Word8 -> [Word8] -> [Word8]
encryptByte x [] = []
encryptByte b (x:xs) = (xor b x):(encryptByte b xs)

encryptMult [] xs = ([], xs)
encryptMult _ [] = ([], [])
encryptMult (x:xs) (y:ys) = (partial, rest)
   where (sub, rest) = encryptMult xs ys
         partial = (xor x y):sub

encryptRepByte :: [Word8] -> [Word8] -> [Word8]
encryptRepByte _ [] = []
encryptRepByte k buff = result
   where (block, rest) = encryptMult k buff
         result = block ++ encryptRepByte k rest

letters :: [Char]
letters = "ETAOIN SHRDLU"

score c = case Data.List.findIndex ((==) ((Data.Char.toUpper . toEnum . fromIntegral) c)) letters of
            Just n -> 13-n
            Nothing -> 0

scoreGuess b str =
   Data.List.foldr (+) 0 scoreVals
   where scoreVals = Data.List.map score encBuff
         encBuff = encryptByte b str
