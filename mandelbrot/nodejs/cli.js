#!/usr/bin/env node
/**
 * CLI application for generating Mandelbrot set data
 */

const { program } = require('commander');
const fs = require('fs');
const mandelbrot = require('./mandelbrot');

function myParseInt(value, previous) {
  // Ensure radix used is 10
  return parseInt(value, 10);
}

// Define the command line interface
program
  .name('mandelbrot')
  .description('CLI to generate data for visualizing the Mandelbrot set')
  .version('0.1.0')
  .option('-x, --center-x <center-x>', 'x-coordinate of the center of the view', parseFloat, -0.5)
  .option('-y, --center-y <center-y>', 'y-coordinate of the center of the view', parseFloat, 0)
  .option('-w, --width <width>', 'width in pixels of the result', myParseInt, 300)
  .option('-h, --height <height>', 'height in pixels of the result', myParseInt, 200)
  .option('-s, --scale <scale>', 'scale factor (size of a pixel)', parseFloat, 0.0075)
  .option('-n, --max-iterations <max-iterations>', 'maximum number of iterations to perform at each pixel', myParseInt, 100)
  .option('-b, --bound <bound>', 'divergence cutoff. If the abs(z) >= bound, the iterations stop', parseFloat, 2)
  .option('-p, --power <power>', 'exponent used in the mandelbrot equation', myParseInt, 2)
  .option('-o, --output <output>', 'output file path (defaults to stdout if not specified)')
  .option('-q, --quiet', 'suppress informational messages')
  .parse(process.argv);

const options = program.opts();

// Generate the Mandelbrot set data
try {
  if (!options.quiet) {
    console.error('Generating Mandelbrot set data...');
  }
  
  const data = mandelbrot.generateMandelbrotData({
    centerX: options.centerX,
    centerY: options.centerY,
    width: options.width,
    height: options.height,
    scale: options.scale,
    maxIterations: options.maxIterations,
    bound: options.bound,
    power: options.power
  });
  
  // JSON data string
  const jsonData = JSON.stringify(data, null, 2);
  
  if (options.output) {
    // Write the data to a JSON file
    fs.writeFileSync(options.output, jsonData);

    if (!options.quiet) {
      console.error(`Successfully wrote data to ${options.output}`);
      console.error(`Generated ${data.points.length} points in the Mandelbrot set`);
      console.error(`Metadata: ${JSON.stringify(data.metadata, null, 2)}`);
    }
  } else {
    // Write to stdout
    process.stdout.write(jsonData);
    
    // Write metadata to stderr so it doesn't interfere with the JSON output
    if (!options.quiet) {
      console.error(`\nGenerated ${data.points.length} points in the Mandelbrot set`);
      console.error(`Metadata: ${JSON.stringify(data.metadata, null, 2)}`);
    }
  }
} catch (error) {
  console.error('An error occurred while generating the Mandelbrot set data:');
  console.error(error);
  process.exit(1);
}
