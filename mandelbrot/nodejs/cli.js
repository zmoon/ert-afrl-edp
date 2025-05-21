#!/usr/bin/env node

/**
 * CLI application for generating Mandelbrot set data
 */

const { program } = require('commander');
const fs = require('fs');
const mandelbrot = require('./mandelbrot');

// Define the command line interface
program
  .name('mandelbrot')
  .description('CLI to generate data for visualizing the Mandelbrot set')
  .version('0.0.0')
  .requiredOption('-x, --center-x <center-x>', 'x-coordinate of the center of the view', parseFloat)
  .requiredOption('-y, --center-y <center-y>', 'y-coordinate of the center of the view', parseFloat)
  .requiredOption('-w, --width <width>', 'width of the view', parseFloat)
  .requiredOption('-h, --height <height>', 'height of the view', parseFloat)
  .requiredOption('-s, --step <step>', 'resolution of the view (size of a pixel)', parseFloat)
  .option('-n, --max-iterations <max-iterations>', 'maximum number of iterations to perform at each pixel', parseInt, 100)
  .option('-b, --bound <bound>', 'divergence cutoff. If the abs(z) >= bound, the iterations stop', parseFloat, 2)
  .option('-p, --power <power>', 'exponent used in the mandelbrot equation', parseInt, 2)
  .option('-o, --output <output>', 'output file path (defaults to stdout if not specified)')
  .option('-q, --quiet', 'suppress informational messages')
  .parse(process.argv);

const options = program.opts();

// Generate the Mandelbrot set data
try {
  console.log('Generating Mandelbrot set data...');
  
  const data = mandelbrot.generateMandelbrotData({
    centerX: options.centerX,
    centerY: options.centerY,
    width: options.width,
    height: options.height,
    step: options.step,
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
      console.error(`Generated ${data.points.length} points in the Mandelbrot set`);
      console.error(`Metadata: ${JSON.stringify(data.metadata, null, 2)}`);
    }
  }
} catch (error) {
  console.error('An error occurred while generating the Mandelbrot set data:');
  console.error(error);
  process.exit(1);
}
