#version 130

uniform float uWidth;
uniform float uHeight;
uniform int uMaxIterations;
uniform vec2 uCenter;
uniform float uScale;
uniform sampler1D uColorPalette;

out vec4 FragColor;

// A color function that maps the iteration count to a color from the palette
vec4 getColor(int iterations) {
    float t = clamp(float(iterations) / float(uMaxIterations), 0.0, 1.0);
    float scale = 1.0 - smoothstep(0.0, 0.36, t);
    return texture1D(uColorPalette, scale);
}

void main() {
    // Calculate the complex coordinates based on the fragment's position
    vec2 c = vec2((gl_FragCoord.x - uWidth / 2.0) * uScale / uWidth + uCenter.x,
                  (gl_FragCoord.y - uHeight / 2.0) * uScale / uHeight + uCenter.y);

    // Mandelbrot set iteration
    vec2 z = vec2(0.0, 0.0);
    int iterations = 0;

    for (int i = 0; i < uMaxIterations; i++) {
        if (length(z) > 2.0)
            break;

        vec2 zNew = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
        z = zNew + c;

        iterations++;
    }

    // Calculate the color based on the iteration count
    vec4 color = getColor(iterations);

    // Assign the fragment color using the color from the getColor function
    FragColor = vec4(color.rgb, 1.0);
}
