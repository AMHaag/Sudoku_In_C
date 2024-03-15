const fs = require('fs');
const readline = require('readline');

const inputFile = './Sudoku_Data_set/sudoku.csv';
const outputFilePrefix = './Sudoku_Data_set/test_batch_';
const maxLinesPerFile = 1000000;

let currentFileIndex = 1;
let currentLineCount = 0;

// Create the first output file
fs.writeFileSync(`${outputFilePrefix}${currentFileIndex}.txt`, '');
let currentWriteStream = fs.createWriteStream(
  `${outputFilePrefix}${currentFileIndex}.txt`
);

const readStream = fs.createReadStream(inputFile);
const rl = readline.createInterface({
  input: readStream,
  crlfDelay: Infinity
});

rl.on('line', (line) => {
  const commaIndex = line.indexOf(',');
  const modifiedLine = commaIndex !== -1 ? line.slice(0, commaIndex) : line;
  currentWriteStream.write(modifiedLine + '\n');
  currentLineCount++;

  if (currentLineCount >= maxLinesPerFile) {
    currentWriteStream.end();
    currentFileIndex++;
    currentLineCount = 0;

    // Create the next output file
    fs.writeFileSync(`${outputFilePrefix}${currentFileIndex}.txt`, '');
    currentWriteStream = fs.createWriteStream(
      `${outputFilePrefix}${currentFileIndex}.txt`
    );
  }
});

rl.on('close', () => {
  console.log('File processing completed.');
  currentWriteStream.end();
});
