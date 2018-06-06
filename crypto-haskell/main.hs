import Cryptopals.Base64
import Cryptopals.EncXor
import System.Environment
import Data.Word

main = do
   args <- getArgs
   let fname = head args
   contents <- readFile fname
   let lines = lineSplit contents
   let result = detectSingleXor lines
   mapM putStrLn lines
   putStrLn $ result

readBase64file :: String -> IO [Word8]
readBase64file fname =  do
   contents <- readFile fname
   let lines = lineSplit contents
   let byteLines = map base64decode lines
   let bytes = concat byteLines
   return bytes

quicksort [] = []
quicksort (x:[]) = [x]
quicksort (x:xs) = (quicksort [(a,v) | (a,v) <- xs, a < k]) ++ [(k,v)]  ++ 
                 (quicksort [(b,z) | (b,z) <- xs, b >= k] )
   where (k, v) = x

doTheThing = do
   bytes <- readBase64file "6.txt"
   let weights = map (\x -> (scoreBlockSize x bytes, x)) [x | x <- [2..40] ]
   let candidates = [bs | (s, bs) <- (take 5 $ quicksort weights)]
   mapM (putStrLn . show) candidates
   let results = map (\x -> solveProjection x (bsProjection x bytes)) candidates
   let reorderd = map (\x -> reassemble (map getbytes x)) results
   return $ map bin2str reorderd
   --return $ bin2str val
      where getbytes (n,s) = s

solveProjection bs lines =
   map crackSingleXor lines

sumSamples 0 _ = 0
sumSamples n (x:y:xs) = (editDistance x y) + (sumSamples (n-1) xs)

avgDistance ns bytes = fromIntegral (sumSamples ns bytes) / ns / 8

lineSplit :: String -> [String]
lineSplit [] = [[]]
lineSplit (c:str) = if c == '\n' then []:(lineSplit str)
                                 else (c:h):t
                                    where rest = lineSplit str
                                          h = head rest
                                          t = tail rest

resultCrack str = result
   where a = hex2bin str
         (key, value) = crackSingleXor a
         result = map (toEnum . fromIntegral) value :: [Char]

crackSingleXor str = best
   where keys = [x | x <- [0..255] ]
         guesses = map (\x -> (scoreGuess x str, encryptByte x str)) keys
         best = maximum guesses

detectSingleXor :: [String] -> String
detectSingleXor strs = bin2str bestResult
   where (key, bestResult) = maximum allGuesses
         allGuesses = map (crackSingleXor . hex2bin) strs

createBlocks :: Int -> [Word8] -> [[Word8]]
createBlocks _ [] = []
createBlocks bs bytes = (take bs bytes):(createBlocks bs (Prelude.drop bs bytes))

scoreBlockSize bs bytes = distance / (fromIntegral bs)
   where blocks = createBlocks bs bytes
         distance = avgDistance 4 blocks


bsProjection bs [] = [[] | x <- [0..bs]]
bsProjection bs list = applyProjection (makeProjection bs list) (bsProjection bs (drop bs list))
applyProjection [] _ = []
applyProjection _ [] = []
applyProjection (p:xs) (e:list) = (p e):(applyProjection xs list)
makeProjection bs list = makePrepend (take bs list)
makePrepend list = map prepend list
prepend s = (\x -> s:x)

reassemble :: [[Word8]] -> [Word8]
reassemble ([]:_) = []
reassemble list = (foldr (++) [] (map (take 1) list)) ++ (reassemble (map (drop 1) list))
