# coding=utf-8
"""
Assignment 3: Fractal shaders, Julia sets
Author: Cristóbal Alcázar, CC7515, 2023-1
"""
import time
import argparse
import cv2
import glfw
import numpy as np

from OpenGL.GL import *
from OpenGL.GL.shaders import compileShader, compileProgram


from utils import read_shader_file, load_texture, draw_quad


# Argparser
# ------------------------------------------------------------------------------ 
# Create an argument parser
parser = argparse.ArgumentParser(description='Mandelbrot set shader')
# Add arguments for width, height, and max_iterations
parser.add_argument('--width', type=int, default=1020, help='Width of the window')
parser.add_argument('--height', type=int, default=764, help='Height of the window')
parser.add_argument('--max_iterations', type=int, default=100, help='Maximum number of iterations')
parser.add_argument('--cx', type=float, default=-0.5, help='x-component of constant seed')
parser.add_argument('--cy', type=float, default=0.0, help='y-component of constant seed')
parser.add_argument('--mincolor', type=float, default=0.0, help='min color for smoothstep to interpolate')
parser.add_argument('--maxcolor', type=float, default=0.35, help='max color for smoothstep to interpolate')
parser.add_argument('--speed', type=float, default=1.25, help='speed of the animation')
parser.add_argument('--out', type=str, default=None, help='Save a MP4 video')
# Parse the command-line arguments
args = parser.parse_args()


# Configuration
# ------------------------------------------------------------------------------ 
WIDTH, HEIGHT = args.width, args.height # Dimensions of the window
QUAD_MARGIN = 0.1   # Adjust the magin size as needed

# Julia set parameters
MAX_ITERATIONS = args.max_iterations
CONSTANT_X, CONSTANT_Y = args.cx, args.cy
CENTER_X, CENTER_Y = 0.0, 0.0
MIN_COLOR, MAX_COLOR = args.mincolor, args.maxcolor
SPEED = args.speed
OUTPUT_VIDEO = args.out
ZOOM_LEVEL = 5.00
VERTEX_SHADER_PATH = "./shaders/vertex_shader.glsl"
FRAGMENT_SHADER_PATH = "./shaders/julia_shader.glsl"
TEXTURE_PATH = "./pal.ppm"

# control parameters via callbacks
ZOOM_SPEED = 0.1    # Adjust the zoom speed as needed


# Callbacks for glfw
# ------------------------------------------------------------------------------ 
def scroll_callback(window, x_offset, y_offset, scale_location):
    """
        Callback function for zoom in and out using the mouse scroll 
    """
    global ZOOM_LEVEL
    # Increase or decrease the zoom level based on scroll direction
    if y_offset > 0:
        ZOOM_LEVEL /= (1.0 + ZOOM_SPEED)
    else:
        ZOOM_LEVEL *= (1.0 + ZOOM_SPEED) 

    # Set the uniform value for zoom scale
    glUniform1f(scale_location, ZOOM_LEVEL)


# Main program
# ------------------------------------------------------------------------------
def main():

    print(f"Width: {args.width}, Height: {args.height}, Max iterations: {args.max_iterations}")

    # Initialize GLFW
    if not glfw.init():
        return

    # Create a window
    #window = glfw.create_window(WIDTH, HEIGHT, f"Julia Shader with c=({CONSTANT_X}, {CONSTANT_Y})", None, None)
    window = glfw.create_window(WIDTH, HEIGHT, "Julia Shader", None, None)

    if not window:
        glfw.terminate()
        return

    # Make the window's OpenGL context current
    glfw.make_context_current(window)

    # Create a video writer
    #-----------------------------------------------------------------------
    if OUTPUT_VIDEO is not None:
        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
        out = cv2.VideoWriter(OUTPUT_VIDEO + '.mov', fourcc, 60.0, (WIDTH, HEIGHT))
    #-----------------------------------------------------------------------

    # Load the color palette texture
    color_palette_texture = load_texture(TEXTURE_PATH)

    # Loader the vertex
    vertex_shader = read_shader_file(VERTEX_SHADER_PATH)
    fragment_shader = read_shader_file(FRAGMENT_SHADER_PATH)

    # Create the shader program: first compile shaders, the compile the program 
    vertex_shader_id = compileShader(vertex_shader, GL_VERTEX_SHADER)
    fragment_shader_id = compileShader(fragment_shader, GL_FRAGMENT_SHADER)
    shader_program_id = compileProgram(vertex_shader_id, fragment_shader_id)

    # Use the shader program
    glUseProgram(shader_program_id)

    # Get the uniform locations
    width_location = glGetUniformLocation(shader_program_id, "uWidth")
    height_location = glGetUniformLocation(shader_program_id, "uHeight")
    max_iterations_location = glGetUniformLocation(shader_program_id, "uMaxIterations")
    constant_seed_location = glGetUniformLocation(shader_program_id, "uConstant")
    color_palette_location = glGetUniformLocation(shader_program_id, "uColorPalette")
    scale_location = glGetUniformLocation(shader_program_id, "uScale")
    center_location = glGetUniformLocation(shader_program_id, "uCenter")
    color_interpolation_location = glGetUniformLocation(shader_program_id, "uColorInterpolation")

    # Set the uniform values
    glUniform1f(width_location, WIDTH)
    glUniform1f(height_location, HEIGHT)
    glUniform1i(max_iterations_location, MAX_ITERATIONS)
    glUniform1f(scale_location, ZOOM_LEVEL)
    glUniform2f(center_location, CENTER_X, CENTER_Y)
    glUniform2f(color_interpolation_location, MIN_COLOR, MAX_COLOR)

    # Bind the color palette texture to the texture unit 0
    glActiveTexture(GL_TEXTURE0)
    glBindTexture(GL_TEXTURE_1D, color_palette_texture)
    glUniform1i(color_palette_location, 0)  # Set the texture unit to 0

    # Set up the viewport
    glViewport(0, 0, WIDTH, HEIGHT)

    # Set the mouse button callback function
    #glfw.set_mouse_button_callback(window, mouse_button_callback)

    # Main loop
    while not glfw.window_should_close(window):

        # Clear the screen
        glClear(GL_COLOR_BUFFER_BIT)

        # Handle scroll input for zooming
        glfw.set_scroll_callback(window, lambda w, x, y: scroll_callback(w, x, y, scale_location))

        # Set the constant seed
        t = time.time() * SPEED

        CONSTANT_X = (np.sin(np.cos(t / 10) * 10) + np.cos(t * 2.0) / 4.0 + np.sin(t * 3.0) / 6.0) * 0.8
        CONSTANT_Y = (np.cos(np.sin(t / 10) * 10) + np.sin(t * 2.0) / 4.0 + np.cos(t * 3.0) / 6.0) * 0.8
        glUniform2f(constant_seed_location, CONSTANT_X, CONSTANT_Y)

        # Draw a fullscreen quad
        draw_quad(margin=QUAD_MARGIN)

        # Capture the image for the video
        #-----------------------------------------------------------------------
        if OUTPUT_VIDEO is not None:
            # Read the pixel data into a numpy array
            pixels = glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE)
            image_data = np.frombuffer(pixels, dtype=np.uint8)
            image_data = np.reshape(image_data, (HEIGHT, WIDTH, 3))
            image = cv2.cvtColor(image_data, cv2.COLOR_RGB2BGR)

            # Write the image to the video writer
            out.write(image)
        #-----------------------------------------------------------------------

        # Swap front and back buffers
        glfw.swap_buffers(window)

        # Poll for and process events
        glfw.poll_events()

    # Cleanup
    glDeleteProgram(shader_program_id)

    glDeleteTextures(1, [color_palette_texture])

    # Terminate GLFW
    if OUTPUT_VIDEO is not None:
        out.release()
    glfw.terminate()

if __name__ == "__main__":
    main()
