module Cryptopals.Aes (
mSub,
sBox
) where
import Data.Bits
import Data.Word
import Cryptopals.Base64

rotl8 :: Word8 -> Int -> Word8
rotl8 x s = (shiftL x s) .|. ( shiftR x (8 -s))

--mSub 0 r _ = r
mSub 0 r _ = r
mSub n r s = (mSub (n-1) res s2)
   where res = xor r s
         s2 = rotl8 s 1

sBox :: Word8 -> Word8
sBox 0 = 0x63
sBox n = xor 0x63 (mSub 5 0 n)

addRoundKey x = x

shiftRows x = x

mixColumns x = x

subBytes x = x

expandKey x = [x]

round k x = x
