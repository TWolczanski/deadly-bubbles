#version 330

in vec3 fColor;
in vec3 fPos;

out vec3 color;

void main(void) {
    // the checkerboard is N x N
    int N = 13;

    if (fPos.z == 0.0 || fPos.z >= 0.999) {
        int x = int(fPos.x * N);
        int y = int(fPos.y * N);
        int index = y * N + x;
        if (index % 2 == 0) {
            // color = vec3(0.0, 0.0, 0.0);
            color = vec3(1.0, 1.0, 1.0);
        }
        else {
            color = vec3(0.0, 0.0, 0.0);
            // color = fColor;
        }
    }
    else if (fPos.x == 0.0 || fPos.x >= 0.999) {
        int x = int(fPos.z * N);
        int y = int(fPos.y * N);
        int index = y * N + x;
        if (index % 2 == 0) {
            color = vec3(0.0, 0.0, 0.0);
            // color = vec3(1.0, 1.0, 1.0);
        }
        else {
            color = vec3(1.0, 1.0, 1.0);
            // color = fColor;
        }
    }
    else if (fPos.y == 0.0 || fPos.y >= 1.4999) {
        int x = int(fPos.z * N);
        int y = int(fPos.x * N);
        int index = y * N + x;
        if (index % 2 == 0) {
            color = vec3(0.0, 0.0, 0.0);
            // color = vec3(1.0, 1.0, 1.0);
        }
        else {
            color = vec3(1.0, 1.0, 1.0);
            // color = fColor;
        }
    }
}