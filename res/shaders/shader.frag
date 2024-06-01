#version 330 core 

#define MAX_ITERATIONS 500
#define THRESHOLD 1000
          
in vec4 gl_FragCoord;

layout(location = 0) out vec4 color; 

uniform float verticalOffset;
uniform float horizontalOffset;
uniform float zoomScale;
uniform vec2 screenSize;

vec2 multiplyComplex(vec2 a, vec2 b) {
    float real = (a.x * b.x) - (a.y * b.y);
    float imag = (a.x * b.y) + (a.y * b.x);

    return vec2(real, imag);
}

vec2 getPixelCoords() {
    float aspectRatio = screenSize.x/screenSize.y;

    vec2 normalizedCoords = gl_FragCoord.xy/screenSize - 0.5;
    normalizedCoords *= 2;

    vec2 coords = vec2(2.0f, 2.0f/aspectRatio) * zoomScale * normalizedCoords + vec2(horizontalOffset, verticalOffset);
    return coords;
}

float iterateFunc(vec2 coords) {
    int iteration = 0;
    vec2 cVal = coords;
    vec2 zVal = coords;

    while(iteration < MAX_ITERATIONS) {
        zVal = multiplyComplex(zVal, zVal) + cVal;
        
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