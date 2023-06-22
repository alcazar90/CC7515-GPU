"""
    File: utils.py
    Description: Utility functions for the application
"""
import numpy as np

from OpenGL.GL import *
from PIL import Image

def read_shader_file(file_path):
    """Read the shader code from the file and return it as a string"""
    with open(file_path, 'r') as file:
        shader_code = file.read()
    return shader_code


def load_texture(image_path):
    """Load the texture from the given image file"""
    texture_id = glGenTextures(1)
    glBindTexture(GL_TEXTURE_1D, texture_id)

    # Set the texture parameters
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

    # Load the texture image
    image = Image.open(image_path)
    image_data = np.array(image.getdata(), np.float32)
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, image.width, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data)

    return texture_id


def draw_quad(margin):
    glBegin(GL_QUADS)
    glTexCoord2f(0, 0)
    glVertex2f(-1.0 + margin, -1.0 + margin)
    glTexCoord2f(1, 0)
    glVertex2f(1.0 - margin, -1.0 + margin)
    glTexCoord2f(1, 1)
    glVertex2f(1.0 - margin, 1.0 - margin)
    glTexCoord2f(0, 1)
    glVertex2f(-1.0 + margin, 1.0 - margin)
    glEnd()
