#version 130

uniform float uWidth;
uniform float uHeight;
uniform int uMaxIterations;
uniform vec2 uCenter;
uniform float uScale;
uniform sampler1D uColorPalette;
uniform vec2 uConstant;

out vec4 FragColor;

// A color function that maps the iteration count to a color from the palette
vec4 getColor(int iterations) {
    float t = clamp(float(iterations) / float(uMaxIterations), 0.0, 1.0);
    float scale = 1.0 - smoothstep(0.0, 0.36, t);
    return texture1D(uColorPalette, scale);
}

void main() {
    vec2 z = vec2((gl_FragCoord.x - uWidth / 2.0) * uScale / uWidth + uCenter.x,
                  (gl_FragCoord.y - uHeight / 2.0) * uScale / uHeight + uCenter.y);

    int iterations = 0;

    for (int i = 0; i < uMaxIterations; i++) {
		float x = exp(z.x) * cos(z.y) + uConstant.x;
		float y = exp(z.x) * sin(z.y) + uConstant.y;

        if (length(z) > 2.0) 
            break;
        
        z.x = x;
        z.y = y;

        iterations++;
    }

    // Calculate the color based on the iteration count
    vec4 color = getColor(iterations);

    // Assign the fragment color using the color from the getColor function
    FragColor = vec4(color.rgb, 1.0);
}