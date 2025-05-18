// HLSL utility functions for normal (Gaussian) random numbers

// Hash-based pseudo-random number generator
float rand(in float2 uv) {
    // A simple hash function for 2D coordinates
    float2 k = float2(12.9898, 78.233);
    float t = dot(uv, k);
    return frac(sin(t) * 43758.5453);
}

// Box-Muller transform: generates a single standard normal random number
float randn(in float2 uv) {
    float u1 = rand(uv);
    float u2 = rand(uv + 17.0); // Offset to decorrelate
    u1 = max(u1, 1e-6); // Avoid log(0)
    float r = sqrt(-2.0 * log(u1));
    float theta = 6.2831853 * u2; // 2*PI
    return r * cos(theta); // Standard normal (mean 0, std 1)
}

// Box-Muller transform: generates two independent standard normal random numbers
float2 randn2(in float2 uv) {
    float u1 = rand(uv);
    float u2 = rand(uv + 23.0); // Offset to decorrelate
    u1 = max(u1, 1e-6); // Avoid log(0)
    float r = sqrt(-2.0 * log(u1));
    float theta = 6.2831853 * u2; // 2*PI
    return float2(r * cos(theta), r * sin(theta));
}
