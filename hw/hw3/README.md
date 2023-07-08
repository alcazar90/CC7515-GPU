# Assignment 3: Fractal Shaders with OpenGL🧨

Author: Cristóbal Alcázar

<br>

<center>
<img src="./assets/output.gif" alt="Source: Julia Set Dynamics, video created with julia-dyn.py" width="640px">
</center>

<br>

**tl;dr**: In this project...

## Usage

There are two files that generates static images of the Mandelbrot and Julia sets: `mandelbrot.py` and `julia.py`. 

```bash
python mandelbrot.py --width 680 --height 680 --max_iteration 50
python mandelbrot.py --width 680 --height 680 --max_iteration 75
python mandelbrot.py --width 680 --height 680 --max_iteration 100
python julia.py --max_iterations 75 --cx -0.8 --cy -0.156 # standard julia set
python julia.py --max_iterations 75 --cx -0.123 --cy 0.745 # douady's rabbit
python julia.py --max_iterations 75 --cx -0.75 --cy 0.11 # seahorse valley
python julia.py --max_iterations 75 --cx -0.74543 --cy 0.11301 # triple spiral
python julia.py --max_iterations 75 --cx -0.745 --cy 0.1135 # devil's claws
```
You can generate images like this:
<table border=0 align=center>
    <tbody>
        <tr>
     	    <td width="27%" align="center"> <img src="./assets/mandelbrot.png"> <code>mandelbrot.py</code></td>
			<td width="27%" align="center"> <img src="./assets/julia-set-triple-spiral.png"><code>julia.py</code></td>
			<td width="27%" align="center"> <img src="./assets/julia-set-2.png" alt="Julia Set c=0.355534,-0.337292"><code>julia.py</code></td>       
        </tr>
    </tbody>
</table>

```bash
# standard julia shader
python julia-dyn.py --max_iterations 160 --julia_shader julia_shader --cx 0.8 --cy -1.2 --speed 0.5
# julia shader with cosine
python julia-dyn.py --max_iterations 60 --julia_shader julia-cos_shader --cx 0.8 --cy -1.2 --speed 1.8
python julia-dyn.py --max_iterations 40 --julia_shader julia-exp_shader --cx 0.8 --cy -1.2
```
There is also a file that generates a video of the Julia set: `julia-dyn.py`. For instance, we can save a video called `lala.mp4` using the flag `--out`:

```bash
python julia-dyn.py --max_iterations 250 --width 1260 --mincolor -0.066 --maxcolor 0.56777 --speed 1.45 --out lala
```

Then, convert it into a gif like the one above using ffmpeg:

```bash
ffmpeg -i lala.mp4 -vf "fps=30,scale=640:-1:flags=lanczos" -c:v gif output.gif
```

## Math Background: Holomorphic Dynamics

Holomorphic refers for a function that maps complex numbers input to complex numbers output.

$$
f:\mathbb{C}\rightarrow\mathbb{C}
$$

The derivative of a holomorphic function exists at every point in its domain, and is itself a holomorphic function.


For some function $f(z)$, the Mandelbrot set is the set of complex numbers $c$ for which the iterated function $f(z)$ does not diverge when starting with $z=0$.

Dynamics refers that we apply the function over and over and over...

$$
z_{0}\rightarrow z_{1}\rightarrow z_{2}\rightarrow\dots \rightarrow z_{n+1}
$$

$$
z_{n+1} = f(z_{n})
$$

### Rational Functions

A function is rational if it can be expressed as the quotient of two polynomials, such as

$$
f(z) = \frac{2z^3 + 1}{3z^2}
$$ 

or, more generally,

$$
f(z) = \frac{a_{n}z^{n}+\dots+a_{0}}{b_{m}z^{m}+\dots + b_{0}}
$$

What happens when we iterate as before some rational functions? Let's start with the simple function $z_{n+1} = z_{n}^{2} + c$, where $c$ is a constant that can be changed. For instance, could be $c=-0.52 + 0.35i$. We can start with an initial $z_{0}=0$ , so

$$
\begin{equation}
\begin{split}
z_{0} &= 0 \\
z_{1} &= 0^{2} + c = c \\
z_{2} &= c^{2} + c \\
z_{3} &= (c^{2} + c)^{2} + c \\
z_{4} &= ((c^2 + c)^2 + c)^2 + c \\
\vdots
\end{split}
\end{equation}
$$

For some values of $c$, the sequence $z_{n}$ will diverge to infinity. For others, it will converge to a fixed point. For others, it will converge to a cycle of length 2, 3, 4, etc. For instance, if $c=0$, then $z_{n}=0$ for all $n$. If $c=1$, then $z_{n}=1$ for all $n$. If $c=-1$, then $z_{n}$ will alternate between $-1$ and $0$.

 
### The Mandelbrot set: $z_{0}$ is fixed

We can define the set of complex numbers $c$ for which the iterated function $f(z)$ does not diverge and stay bounded. This is the Mandelbrot set.

### The Julia set: $z_{0}$ is not fixed

<center>
<img src="./assets/mandelbrot-julia-3b1b.png" alt="Source: Beyond the Mandelbrot set, an intro to holomorphic dynamics, 3Blue1Brown video" width="600px">
</center>


## Vertex Pipeline

<center>
<img src="./assets/vertex-pipeline-graphicShaderBook.png" alt="Graphic Shaders, by Mike Bailey and Steve Cunningham" width="360">
</center>

**TODO:** Fractals are used heavily uniform variables across the vertex and fragment shaders. The vertex shader is responsible for transforming the vertices of the geometry into screen space. The vertex shader is also responsible for passing the uniform variables to the fragment shader.


```C
#version 130

uniform float uWidth;
uniform float uHeight;
uniform int uMaxIterations;
uniform vec2 uCenter;
uniform float uScale;
uniform sampler1D uColorPalette;
uniform vec2 uConstant;
uniform vec2 uColorInterpolation;

out vec4 FragColor;

// A color function that maps the iteration count to a color from the palette
vec4 getColor(int iterations) {
    float t = clamp(float(iterations) / float(uMaxIterations), 0.0, 1.0);
    //float scale = 1.0 - smoothstep(0.0, 0.56, t);
    float scale = 1.0 - smoothstep(uColorInterpolation.x, uColorInterpolation.y, t);
    return texture1D(uColorPalette, scale);
}

void main() {
    vec2 z = vec2((gl_FragCoord.x - uWidth / 2.0) * uScale / uWidth + uCenter.x,
                  (gl_FragCoord.y - uHeight / 2.0) * uScale / uHeight + uCenter.y);

    int iterations = 0;

    for (int i = 0; i < uMaxIterations; i++) {
        float x = (z.x * z.x - z.y * z.y) + uConstant.x;
        float y = (z.y * z.x + z.x * z.y) + uConstant.y;

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
```

## Dependencies

Create a new python environment with `python>=3.8` and install the dependencies:

```bash
pip install -r requirements.txt
```


## Resources

* [Beyond the Mandelbrot set, an intro to holomorphic dynamics - 3Blue1Brown](https://www.youtube.com/watch?v=LqbZpur38nw)
* [How to Fold a Julia Fractal, a tale of numbers that like to turn - Steven Wittens](https://acko.net/blog/how-to-fold-a-julia-fractal/)
* [Fast and Easy High Resolution Fractals with a Pixel Shader - John Tsiombikas](http://nuclear.mutantstargoat.com/articles/sdr_fract/)
* [Julia Set Fractal (2D) - Paul Bourke](http://paulbourke.net/fractals/juliaset/)
