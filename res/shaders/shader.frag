#version 330 core 

#define MAX_ITERATIONS 50
#define THRESHOLD 5
          
in vec4 gl_FragCoord;

layout(location = 0) out vec4 color; 

vec2 multiplyComplex(vec2 a, vec2 b) {
    float real = (a.x * b.x) - (a.y * b.y);
    float imag = (a.x * b.y) + (a.y * b.x);

    return vec2(real, imag);
}

vec2 getPixelCoords() {
    return vec2((gl_FragCoord.x/900.0f) * 3 - 2.0f, (gl_FragCoord.y/900.0f) * 3 - 1.5f);
}

float iterateFunc(vec2 coords) {
    int iteration = 0;
    float c_real = coords.x;
    float c_imag = coords.y;
    vec2 zVal = coords;

    while(iteration < MAX_ITERATIONS) {
        zVal = multiplyComplex(zVal, zVal) + vec2(c_real, c_imag);
        
        float dist = zVal.x * zVal.x + zVal.y * zVal.y;
        if(dist > THRESHOLD) {
            return iteration;
        }

        iteration++;
    }

    return iteration;
}

void main() { 
    vec2 coords = getPixelCoords();
    float iter = iterateFunc(coords);

    if (iter == MAX_ITERATIONS)
    {
        gl_FragDepth = 0.0f;
        color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    } 
    else {
        // Greyscale
        float iterations = float(iter) / float(MAX_ITERATIONS);

        // Log greyscale
        // float iterations = log(float(iter)) / log(float(MAX_ITERATIONS));    

        color = vec4(iterations, iterations, iterations, 1.0f); 
    }
};