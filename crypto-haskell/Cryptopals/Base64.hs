module Cryptopals.Base64(
base64,
hex2bin,
bin2hex,
str2bin,
bin2str,
editDistance,
base64decode
) where
import System.Environment
import Data.ByteString
import Data.Word
import Data.Bits
import Data.Char

main = Prelude.putStrLn "Do stuff"

b64 :: Word8 -> Char
b64 c
   | c < 26 = toEnum (fromEnum 'A' + (fromIntegral c ))
   | c < 52 = toEnum (fromEnum 'a' + (fromIntegral c) - 26)
   | c < 62 = toEnum (fromEnum '0' + (fromIntegral c) - 52)
   | c == 62 = '+'
   | c == 63 = '/'

   | otherwise = '/'

b64c2b :: Char -> Word8
b64c2b c
   | c == '/' = 63
   | c == '+' = 62
   | (fromEnum c) >= (fromEnum 'a') = (fromIntegral . fromEnum) c - ((fromIntegral . fromEnum) 'a') + 26
   | (fromEnum c) >= (fromEnum 'A') = (fromIntegral . fromEnum) c - ((fromIntegral . fromEnum) 'A')
   | (fromEnum c) >= (fromEnum '0') = (fromIntegral . fromEnum) c - ((fromIntegral . fromEnum) '0') + 52
   | otherwise = 0

base64 :: [Word8] -> String
base64 [] = []
base64 (x:[]) = (b64 (shiftR x 2)):(b64 (shift (x .&. 0x3) 4)):'=':'=':[]
base64 (x:y:[]) = (b64 (shiftR x 2)):
                  (b64 ( (shift (x .&. 0x3) 4) .|. (shiftR (y .&. 0xf0) 4))):
                  (b64 (shift (y .&. 0xf) 2)):'=':[]
base64 (x:y:z:xs) = (b64 (shiftR x 2)):
                     (b64 ( (shift (x .&. 0x3) 4) .|. (shiftR (y .&. 0xf0) 4))):
                     (b64 ((shift (y .&. 0x0f) 2) .|. ( shiftR (z .&. 0xc0) 6))):
                     (b64 (z .&. 0x3f)):(base64 xs)

base64decode :: String -> [Word8]
base64decode [] = []
base64decode (x:y:'=':'=':[]) = first:[]
   where first = (shiftL (b64c2b x) 2) .|. ( shiftR ((b64c2b y) .&. 0x30) 4 )
base64decode (x:y:z:'=':[]) = first:second:[]
   where first = (shiftL (b64c2b x) 2) .|. ( shiftR ((b64c2b y) .&. 0x30) 4 )
         second = (shiftL ((b64c2b y) .&. 0x0f) 4) .|. (shiftR ((b64c2b z) .&. 0x3c) 2)
base64decode (x:y:z:w:xs) = first:second:third:(base64decode xs)
   where first = (shiftL (b64c2b x) 2) .|. ( shiftR ((b64c2b y) .&. 0x30) 4 )
         second = (shiftL ((b64c2b y) .&. 0x0f) 4) .|. (shiftR ((b64c2b z) .&. 0x3c) 2)
         third = (shiftL ((b64c2b z) .&. 0x03) 6) .|. ((b64c2b w) .&. 0x3f)


c2b :: Char -> Word8
c2b c
   | c == '0' = 0
   | c == '1' = 1
   | c == '2' = 2
   | c == '3' = 3
   | c == '4' = 4
   | c == '5' = 5
   | c == '6' = 6
   | c == '7' = 7
   | c == '8' = 8
   | c == '9' = 9
   | c == 'A' || c == 'a' = 10
   | c == 'B' || c == 'b' = 11
   | c == 'C' || c == 'c' = 12
   | c == 'D' || c == 'd' = 13
   | c == 'E' || c == 'e' = 14
   | c == 'F' || c == 'f' = 15
   | otherwise = 0

hex2bin :: String -> [Word8]
hex2bin [] = []
hex2bin (x:y:xs) = ((shift (c2b x) 4) .|. (c2b y)):hex2bin xs

bin2hex :: [Word8] -> String
bin2hex [] = []
bin2hex (x:xs) = (intToDigit . fromIntegral $ (shiftR x 4)) : (intToDigit . fromIntegral $ (x .&. 0xf)):(bin2hex xs)

str2bin :: String -> [Word8]
str2bin str = bytes
         where ints = Prelude.map fromEnum str
               bytes = Prelude.map fromIntegral ints :: [Word8]

bin2str :: [Word8] -> String
bin2str bin = Prelude.map (toEnum . fromIntegral) bin :: [Char]

editDistance :: [Word8] -> [Word8] -> Int
editDistance [] [] = 0
editDistance (x:xs) [] = 8 + (editDistance xs [])
editDistance [] (x:xs) = 8 + (editDistance [] xs)
editDistance (x:xs) (y:ys) = (popCount (xor x y)) + (editDistance xs ys)
