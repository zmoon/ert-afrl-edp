/**
 * Mandelbrot set calculator module
 */

/**
 * Calculates the number of iterations before the Mandelbrot function diverges
 * @param {number} x - Real component of the complex number
 * @param {number} y - Imaginary component of the complex number
 * @param {number} maxIterations - Maximum iterations to perform
 * @param {number} bound - Divergence cutoff ("escape radius")
 * @param {number} power - Exponent used in the Mandelbrot equation
 * @returns {number} - Number of iterations before divergence, or maxIterations if it doesn't diverge
 */
function calculatePoint(x, y, maxIterations, bound, power) {
  let real = 0;
  let imag = 0;
  let iteration = 0;

  // We iterate until we reach max iterations or until the point escapes the bound
  while (
    iteration < maxIterations &&
    real * real + imag * imag < bound * bound
  ) {
    // Calculate the next iteration based on the formula: z = z^power + c
    if (power === 2) {
      // The standard Mandelbrot equation z = z^2 + c
      const newReal = real * real - imag * imag + x;
      imag = 2 * real * imag + y;
      real = newReal;
    } else {
      // For higher powers, we need to calculate z^power
      // Use the complex number power formula: (r*e^(i*θ))^n = r^n * e^(i*n*θ)
      const r = Math.sqrt(real * real + imag * imag);
      let theta = 0;

      // Avoid division by zero
      if (r > 0) {
        theta = Math.atan2(imag, real);
      }

      const rPow = Math.pow(r, power);
      const newTheta = theta * power;

      real = rPow * Math.cos(newTheta) + x;
      imag = rPow * Math.sin(newTheta) + y;
    }

    iteration++;
  }

  return iteration;
}

/**
 * Generates Mandelbrot set data for a given viewport
 * @param {Object} options - The generation options
 * @param {number} options.centerX - X-coordinate of the center
 * @param {number} options.centerY - Y-coordinate of the center
 * @param {number} options.width - Width in pixels of the result
 * @param {number} options.height - Height in pixels of the result
 * @param {number} options.scale - Scale factor (size of a pixel in complex plane units)
 * @param {number} options.maxIterations - Maximum iterations to perform
 * @param {number} options.bound - Divergence cutoff ("escape radius")
 * @param {number} options.power - Exponent used in the formula
 * @returns {Object} - The Mandelbrot data
 */
function generateMandelbrotData(options) {
  const {
    centerX,
    centerY,
    width,
    height,
    scale,
    maxIterations,
    bound,
    power,
  } = options;

  // Calculate the total view dimensions in the complex plane
  const viewWidth = width * scale;
  const viewHeight = height * scale;

  // Calculate the boundary pixel centers
  const minX = centerX - viewWidth / 2 + scale / 2;
  const maxX = centerX + viewWidth / 2 - scale / 2;
  const minY = centerY - viewHeight / 2 + scale / 2;
  const maxY = centerY + viewHeight / 2 - scale / 2;

  // Number of points in each dimension are now directly width and height
  const numX = width;
  const numY = height;
  const result = {
    metadata: {
      // Input
      centerX,
      centerY,
      width,
      height,
      scale,
      maxIterations,
      bound,
      power,
      // Derived
      numX,
      numY,
      minX,
      maxX,
      minY,
      maxY,
    },
    points: [],
  };

  // Calculate all points in the viewport
  for (let row = 0; row < numY; row++) {
    // Calculate y coordinate in the complex plane (imaginary part)
    const y = maxY - row * scale;
    for (let col = 0; col < numX; col++) {
      // Calculate x coordinate in the complex plane (real part)
      const x = minX + col * scale;
      const iterations = calculatePoint(x, y, maxIterations, bound, power);
      result.points.push({
        x,
        y,
        iterations,
      });
    }
  }

  return result;
}

module.exports = {
  calculatePoint,
  generateMandelbrotData,
};
