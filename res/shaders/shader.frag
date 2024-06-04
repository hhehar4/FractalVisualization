#version 400 core 

precision highp float;

#define MAX_ITERATIONS 200
#define THRESHOLD 100
          
in vec4 gl_FragCoord;

layout(location = 0) out vec4 color; 

uniform double verticalOffset;
uniform double horizontalOffset;
uniform double zoomScale;
uniform dvec2 screenSize;
uniform bool showJulia;
uniform dvec2 mousePos;

dvec2 multiplyComplex(dvec2 a, dvec2 b) {
    double real = (a.x * b.x) - (a.y * b.y);
    double imag = (a.x * b.y) + (a.y * b.x);

    return dvec2(real, imag);
}

dvec2 getPixelCoords() {
    double aspectRatio = screenSize.x/screenSize.y;

    dvec2 normalizedCoords = (gl_FragCoord.xy/screenSize) - 0.5;
    normalizedCoords *= 2;

    dvec2 coords = dvec2(2.0 * aspectRatio, 2.0) * zoomScale * normalizedCoords + dvec2(horizontalOffset, verticalOffset);
    return coords;
}

vec3 mapToColor(double iterations) {
    vec3 color1 = vec3(24,0,0) / 255.0;
    vec3 color2 = vec3(103,43,33) / 255.0;
    vec3 color3 = vec3(175,69,69) / 255.0;
    vec3 color4 = vec3(199,184,127) / 255.0;
    vec3 color5 = vec3(38,162,152) / 255.0;
    vec3 color6 = vec3(61,157,180) / 255.0;
    vec3 color7 = vec3(12,110,187) / 255.0;
    vec3 color8 = vec3(68,101,189) / 255.0;
    vec3 color9 = vec3(16,136,131) / 255.0;
    vec3 color10 = vec3(37,173,126) / 255.0;
    vec3 color11 = vec3(228,159,148) / 255.0;

    float t = float(iterations / double(MAX_ITERATIONS));

    if (t < 0.05) {
        return mix(color1, color2, t / 0.05);
    } else if (t < 0.1) {
        return mix(color2, color3, (t - 0.05) / 0.05);
    } else if (t < 0.2) {
        return mix(color3, color4, (t - 0.1) / 0.1);
    } else if (t < 0.3) {
        return mix(color4, color5, (t - 0.2) / 0.1);
    } else if (t < 0.5) {
        return mix(color5, color6, (t - 0.3) / 0.2);
    } else if (t < 0.7) {
        return mix(color6, color7, (t - 0.5) / 0.2);
    } else if (t < 0.8) {
        return mix(color7, color8, (t - 0.7) / 0.1);
    } else if (t < 0.9) {
        return mix(color8, color9, (t - 0.8) / 0.1);
    } else if (t < 0.95) {
        return mix(color9, color10, (t - 0.9) / 0.05);
    } else if (t < 1.00) {
        return mix(color10, color11, (t - 0.95) / 0.05);
    } else {
        return vec3(0.0, 0.0, 0.0);
    }
}

double iterateFuncMandelbrot(dvec2 coords) {
    int iteration = 0;
    dvec2 cVal = coords;
    dvec2 zVal = coords;
    double mod = zVal.x * zVal.x + zVal.y * zVal.y;

    while(mod < THRESHOLD && iteration < MAX_ITERATIONS) {
        zVal = multiplyComplex(zVal, zVal) + cVal;
        mod = zVal.x * zVal.x + zVal.y * zVal.y;
        iteration++;
    }

    double smooth_iter = double(iteration) - double(log2(max(1.0f, log2(float(mod)))));
    return smooth_iter;
}

double iterateFuncJulia(dvec2 coords) {
    int iteration = 0;
    dvec2 zVal = coords;
    double mod = zVal.x * zVal.x + zVal.y * zVal.y;

    while(mod < THRESHOLD && iteration < MAX_ITERATIONS) {
        zVal = multiplyComplex(zVal, zVal) + mousePos;
        mod = zVal.x * zVal.x + zVal.y * zVal.y;
        iteration++;
    }

    double smooth_iter = double(iteration) - double(log2(max(1.0f, log2(float(mod)))));
    return smooth_iter;
}

void main() { 
    dvec2 coords = getPixelCoords();
    double iterMandelbrot = iterateFuncMandelbrot(coords);

    vec4 mandelBrotColor = vec4(mapToColor(iterMandelbrot), 1.0f); 
    color = mandelBrotColor;

    if (showJulia) {
        double iterJulia = iterateFuncJulia(coords);
        vec4 juliaColor = vec4(mapToColor(iterJulia), 1.0f); 
        color = mix(juliaColor, mandelBrotColor, 0.25);
    }
};