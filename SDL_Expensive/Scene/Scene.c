//
//  Scene.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Scene.h"

#define max_lights 50
#define max_materials 50
#define max_images 50

Light _lights[max_lights];
Material _materials[max_materials];
SDL_Surface* _images[max_images];

unsigned int numLights = 0;
unsigned int numMaterials = 0;
unsigned int numImages = 0;
Light* lights = _lights;
Material* materials = _materials;

void add_Light(Light light) {
    if (numLights < max_lights) {
        _lights[numLights++] = light;
    }
}

void add_Material(Material material) {
    if (numMaterials < max_materials) {
        _materials[numMaterials++] = material;
    }
}

void addimage(const char * txt) {
    SDL_Surface * image;
    if (numImages < max_materials) {
        image = SDL_LoadBMP(txt);
        if (image == NULL)
            nullErrorCheck(NULL, Scene_not_found_error_mssg);
        _images[numImages++] = image;
    }
}

void load_materials(const char * file_name) {
    FILE* ptr = fopen(file_name, "r");
    if (ptr==NULL) {
        printf("no such file.");
        return;
    }
    
    float acx, acy, acz;
    float dcx, dcy, dcz;
    float scx, scy, scz;
    
    float roughness;
    float fresnel;
    float density;
    
    float reflectivity;
    float refractivity;
    float ior;
    
    float shininess;
    int type;
    int frost;
        
    char AC[50];
    char DC[50];
    char SC[50];
    
    char RG[50];
    char FR[50];
    char DN[50];
    
    char RF[50];
    char RR[50];
    char IO[50];
    
    char SH[50];
    char TP[50];
    char FS[50];
    
    fscanf(ptr,"%s %s %s    %s %s %s    %s %s %s    %s %s %s", AC, DC, SC, RG, FR, DN, RF, RR, IO, SH, TP, FS);
    printf("%s %s %s    %s %s %s    %s %s %s    %s %s %s\n", AC, DC, SC, RG, FR, DN, RF, RR, IO, SH, TP, FS);
    
    while (fscanf(ptr, "%f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %d %d",
                  &acx, &acy, &acz,
                  &dcx, &dcy, &dcz,
                  &scx, &scy, &scz,
                  &roughness, &fresnel, &density,
                  &reflectivity, &refractivity, &ior,
                  &shininess, &type, &frost) != EOF) {
        printf("%f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %d %d\n",
        acx, acy, acz,
        dcx, dcy, dcz,
        scx, scy, scz,
        roughness, fresnel, density,
        reflectivity, refractivity, ior,
               shininess, type, frost);
        simd_float3 ambient_color = {acx, acy, acz};
        simd_float3 diffuse_color = {dcx, dcy, dcz};
        simd_float3 specular_color = {scx, scy, scz};

        Material mat = {ambient_color, diffuse_color, specular_color, roughness, fresnel, density, reflectivity, refractivity, ior, shininess, type, frost};
        add_Material(mat);
    }
    
    return;
}

void load_images(const char * file_name) {
    FILE* ptr = fopen(file_name,"r");
    
    if (ptr==NULL) {
        printf("no such file.");
        return;
    }
    
    char nick[50];
    char url[50];

    fscanf(ptr,"%s %s" , nick, url);
    printf("%s %s\n", nick, url);
    
    while (fscanf(ptr, "%s %s", nick, url) != EOF) {
        printf("%s %s\n", nick, url);
        addimage(url);
    }
    
    return;
}

void load_Triangles(const char * file_name) {
    FILE* ptr = fopen(file_name, "r");
    if (ptr==NULL) {
        printf("no such file.");
        return;
    }
    
    float acx, acy, acz;
    float bcx, bcy, bcz;
    float ccx, ccy, ccz;
    
    int mat;
    int pic;
        
    char AC[50];
    char DC[50];
    char SC[50];
    
    char SH[50];
    char TP[50];
    
    fscanf(ptr,"%s %s %s    %s %s", AC, DC, SC, SH, TP);
    printf("%s %s %s    %s %s\n", AC, DC, SC, SH, TP);
    
    while (fscanf(ptr, "%f %f %f   %f %f %f   %f %f %f  %d %d",
                  &acx, &acy, &acz,
                  &bcx, &bcy, &bcz,
                  &ccx, &ccy, &ccz,
                  &mat, &pic) != EOF) {
        printf("%f %f %f   %f %f %f   %f %f %f  %d %d \n",
        acx, acy, acz,
        bcx, bcy, bcz,
        ccx, ccy, ccz,
        mat, pic);
        simd_float3 Avertice = {acx, acy, acz};
        simd_float3 Bvertice = {bcx, bcy, bcz};
        simd_float3 Cvertice = {ccx, ccy, ccz};

        SDL_Surface* surf = NULL;
        if (pic != -1) {
            surf = _images[pic];
        }
        
        Triangle triangle = makeTri(Avertice, Bvertice, Cvertice, &_materials[mat], surf);
        add_Triangle(triangle);
//        triangle.n *= -1;
//        add_Triangle(triangle);
    }
    
    return;
}

void load_Spheres(const char * file_name) {
    FILE* ptr = fopen(file_name, "r");
    if (ptr==NULL) {
        printf("no such file.");
        return;
    }
    
    float acx, acy, acz;
    float bcx;
    
    int mat;
    int pic;
        
    char AC[50];
    char DC[50];
    char SC[50];
    
    char SH[50];
    
    fscanf(ptr,"%s %s %s %s", AC, DC, SC, SH);
    printf("%s %s %s %s\n", AC, DC, SC, SH);
    
    while (fscanf(ptr, "%f %f %f   %f  %d %d",
                  &acx, &acy, &acz,
                  &bcx,
                  &mat, &pic) != EOF) {
        printf("%f %f %f   %f %d %d \n",
        acx, acy, acz,
        bcx,
        mat, pic);
        simd_float3 Avertice = {acx, acy, acz};

        SDL_Surface* surf = NULL;
        if (pic != -1) {
            surf = _images[pic];
        }
        
        Sphere sphere = makeSph(Avertice, bcx, &_materials[mat], surf);
        add_Sphere(sphere);
    }
    
    return;
}

void load_Planes(const char * file_name) {
    FILE* ptr = fopen(file_name, "r");
    if (ptr==NULL) {
        printf("no such file.");
        return;
    }
    
    float acx, acy, acz;
    float bcx, bcy, bcz;
    
    int mat;
    int pic;
        
    char AC[50];
    char DC[50];
    char SC[50];
    
    char TP[50];
    
    fscanf(ptr,"%s %s %s   %s", AC, DC, SC, TP);
    printf("%s %s %s     %s\n", AC, DC, SC, TP);
    
    while (fscanf(ptr, "%f %f %f   %f %f %f  %d %d",
                  &acx, &acy, &acz,
                  &bcx, &bcy, &bcz,
                  &mat, &pic) != EOF) {
        printf("%f %f %f   %f %f %f   %d %d \n",
        acx, acy, acz,
        bcx, bcy, bcz,
        mat, pic);
        simd_float3 Avertice = {acx, acy, acz};
        simd_float3 Bvertice = {bcx, bcy, bcz};

        SDL_Surface* surf = NULL;
        if (pic != -1) {
            surf = _images[pic];
        }
        
        Plane plane = makePln(Avertice, Bvertice, &_materials[mat]);
        add_Plane(plane);
    }
    
    return;
}

void init_Scene_OO() {
    //Lights
    simd_float3 lcvec = {1.0f, 1.0f, 1.0f};
    simd_float3 lpvec = {0.0f, 4.0f, -5.0f};
    simd_float3 ldvec = {0.0f, 0.0f, 0.0f};
    Light light = {lcvec, lpvec, ldvec, 1.0f, 0.0f, 0.0f, 0.1f};
    add_Light(light);

    //Materials
    load_materials("Materials.txt");
    
    //images
    load_images("Pictures.txt");

    //Objects
    load_Triangles("Triangles.txt");
//    triangles[0].n = simd_make_float3(0, 0, -1);
//    load_Spheres("Spheres.txt");
    load_Planes("Planes.txt");
    
}

void init_Scene() {
    init_Scene_OO();
}
