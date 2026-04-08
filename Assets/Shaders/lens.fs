#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 center;    
uniform float radius;   
uniform float strength; 

void main() {
    vec2 pos = gl_FragCoord.xy; 
    vec2 dir = pos - center;
    float dist = length(dir);
    
    if ( dist < radius && strength > 0.0) {
        float percent = dist / radius;
        
        // x -> sqrt(1 - x²) fait un beau tarpé
        float force = sqrt(1 - percent * percent);
        
        vec2 norm_dir = dir / radius; 
        
        vec2 uv_offset = norm_dir * force * strength * 0.02; 
        
        finalColor = texture(texture0, fragTexCoord - uv_offset);
    } else {
        // En dehors de la bulle, on affiche l'image normalement
        finalColor = texture(texture0, fragTexCoord);
    }
}