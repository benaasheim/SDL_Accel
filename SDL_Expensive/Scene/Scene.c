//
//  Scene.c
//  SDL_Expensive
//
//  Created by Ben on 9/3/20.
//  Copyright © 2020 Ben. All rights reserved.
//

#include "Scene.h"

#define max_lights 50
#define max_objects 100
#define max_materials 50
#define max_images 50

Object _objects[max_objects];
Light _lights[max_lights];
Material _materials[max_materials];
SDL_Surface* _images[max_images];

unsigned int numObjects = 0;
unsigned int numLights = 0;
unsigned int numMaterials = 0;
unsigned int numImages = 0;
Object* objects = _objects;
Light* lights = _lights;
Material* materials = _materials;

void add_Light(Light light) {
    if (numLights < max_lights) {
        _lights[numLights++] = light;
    }
}

void add_Object(Object object) {
    if (numObjects < max_objects) {
        _objects[numObjects++] = object;
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

void init_Scene1() {
    //Lights
    simd_float3 lcvec = {1.0f, 1.0f, 1.0f};
    simd_float3 lpvec = {0.0f, 4.0f, -5.0f};
    simd_float3 ldvec = {0.0f, 0.0f, 0.0f};
    Light light = {lcvec, lpvec, ldvec, 1.0f, 0.0f, 0.0f, 0.1f};
    add_Light(light);

    //Materials
    add_Material(Diffuse_White());
    add_Material(Diffuse_Red());
    add_Material(Diffuse_Blue());
    add_Material(Reflective_Metal());
    add_Material(Refractive_Glass());

    //Objects
    simd_float3 svec1 = {2, 0.75, -2};
    simd_float3 svec2 = {0, 0.75, -3};
    simd_float3 svec3 = {-2, 0.75, -4};
//    add_Object(makeSphere(svec1, 0.75, 3));
//    add_Object(makeSphere(svec2, 0.25, 4));
//    add_Object(makeSphere(svec3, 0.25, 4));
    simd_float3 pvec1 = {0, 0, 0};
    simd_float3 pvec2 = {0, 5, 0};
    simd_float3 pvec3 = {0, 0, -10};
    simd_float3 pvec4 = {4, 0, 0};
    simd_float3 pvec5 = {-4, 0, 0};
    simd_float3 pvec11 = {0, 1, 0};
    simd_float3 pvec12 = {0, -1, 0};
    simd_float3 pvec13 = {0, 0, 1};
    simd_float3 pvec14 = {-1, 0, 0};
    simd_float3 svec15 = {1, 0, 0};
    add_Object(makePlane(pvec1, pvec11, 0));
    add_Object(makePlane(pvec2, pvec12, 0));
    add_Object(makePlane(pvec3, pvec13, 0));
    add_Object(makePlane(pvec4, pvec14, 1));
    add_Object(makePlane(pvec5, svec15, 2));
}

void init_Scene2() {
    //Lights
    simd_float3 lcvec = {1.0f, 0.7f, 0.5f};
    simd_float3 lpvec = {2.0f, 450.0f, -24.0f};
    simd_float3 ldvec = {0.0f, 0.0f, 0.0f};
    add_Light(
//              newLight(lcvec, lpvec, 1.0f, 0.0f, 0.0f, 0.1f)
              (Light){lcvec, lpvec, ldvec, 100000.0f, 0.0f, 0.0f, 0.9f}
              );


    //Materials
    add_Material(Diffuse_White());
    add_Material(Diffuse_Red());
    add_Material(Diffuse_Blue());
    add_Material(Reflective_Metal());
    add_Material(Refractive_Glass());
    add_Material(Diffuse_Sky_Blue());

    //Objects
    simd_float3 svec1 = {2, 0.75, -2};
    simd_float3 svec2 = {0, 0.75, -3};
    simd_float3 svec3 = {-2, 0.75, -4};
//    add_Object(makeSphere(svec1, 0.75, 3));
//    add_Object(makeSphere(svec2, 0.25, 4));
//    add_Object(makeSphere(svec3, 0.25, 4));
    simd_float3 pvec1 = {0, 0, 0};
    simd_float3 pvec2 = {0, 500, 0};
    simd_float3 pvec11 = {0, 1, 0};
    simd_float3 pvec12 = {0, -1, 0};
    add_Object(makePlane(pvec1, pvec11, 0));
    add_Object(makePlane(pvec2, pvec12, 5));
}

void init_Scene3() {
    //Lights
    simd_float3 lcvec = {1.0f, 1.0f, 1.0f};
    simd_float3 lpvec = {0.0f, 4.0f, -5.0f};
    simd_float3 ldvec = {0.0f, 0.0f, 0.0f};
    Light light = {lcvec, lpvec, ldvec, 1.0f, 0.0f, 0.0f, 0.1f};
    add_Light(light);

    //Materials
    add_Material(Diffuse_White());
    add_Material(Diffuse_Red());
    add_Material(Diffuse_Blue());
    add_Material(Reflective_Metal());
    add_Material(Refractive_Glass());
    add_Material(Diffuse_Sky_Blue());

    //Objects
    simd_float3 svec1 = {2, 0.75, -2};
    simd_float3 svec2 = {0, 0.75, -3};
    simd_float3 svec3 = {-2, 0.75, -4};
//    add_Object(makeSphere(svec1, 0.75, 3));
//    add_Object(makeSphere(svec2, 0.25, 4));
//    add_Object(makeSphere(svec3, 0.25, 4));
    simd_float3 pvec1 = {0, 0, 0};
    simd_float3 pvec2 = {0, 5, 0};
    simd_float3 pvec3 = {0, 0, -10};
    simd_float3 pvec4 = {4, 0, 0};
    simd_float3 pvec5 = {-4, 0, 0};
    simd_float3 pvec11 = {0, 1, 0};
    simd_float3 pvec12 = {0, -1, 0};
    simd_float3 pvec13 = {0, 0, 1};
    simd_float3 pvec14 = {-1, 0, 0};
    simd_float3 svec15 = {1, 0, 0};
    add_Object(makePlane(pvec1, pvec11, 0));
    add_Object(makePlane(pvec2, pvec12, 0));
    add_Object(makePlane(pvec3, pvec13, 0));
    add_Object(makePlane(pvec4, pvec14, 1));
    add_Object(makePlane(pvec5, svec15, 2));

    simd_float3 t1a = {1, 1, -7};
    simd_float3 t1b = {0, 4.5, -7};
    simd_float3 t1c = {-1, 1, -7};
    add_Object(makeTriangle(t1a, t1b, t1c, 5, NULL));
}

void init_Scene4() {
    //Lights
    simd_float3 lcvec = {1.0f, 1.0f, 1.0f};
        simd_float3 lpvec = {0.0f, 4.0f, -5.0f};
        simd_float3 ldvec = {0.0f, 0.0f, 0.0f};
        add_Light(
                  (Light){lcvec, lpvec, ldvec, 1.0f, 0.0f, 0.0f, 0.1f}
                  );

    //Materials
    add_Material(Diffuse_White());
    add_Material(Diffuse_Red());
    add_Material(Diffuse_Blue());
    add_Material(Reflective_Metal());
    add_Material(Refractive_Glass());
    add_Material(Diffuse_Sky_Blue());

    simd_float3 t1a = {1, 1, -7};
    simd_float3 t1b = {0, 4.5, -7};
    simd_float3 t1c = {-1, 1, -7};
    
    SDL_Surface * image
    = SDL_LoadBMP("image.BMP");
    
    if (image == NULL)
    nullErrorCheck(NULL, Scene_not_found_error_mssg);
    Object triangle = makeTriangle(t1a, t1b, t1c, 5, image);
    add_Object(triangle);
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
    
    fscanf(ptr,"%s %s %s    %s %s %s    %s %s %s    %s %s", AC, DC, SC, RG, FR, DN, RF, RR, IO, SH, TP);
    printf("%s %s %s    %s %s %s    %s %s %s    %s %s\n", AC, DC, SC, RG, FR, DN, RF, RR, IO, SH, TP);
    
    while (fscanf(ptr, "%f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %d",
                  &acx, &acy, &acz,
                  &dcx, &dcy, &dcz,
                  &scx, &scy, &scz,
                  &roughness, &fresnel, &density,
                  &reflectivity, &refractivity, &ior,
                  &shininess, &type) != EOF) {
        printf("%f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %f %f   %f %d\n",
        acx, acy, acz,
        dcx, dcy, dcz,
        scx, scy, scz,
        roughness, fresnel, density,
        reflectivity, refractivity, ior,
               shininess, type);
        simd_float3 ambient_color = {acx, acy, acz};
        simd_float3 diffuse_color = {dcx, dcy, dcz};
        simd_float3 specular_color = {scx, scy, scz};

        Material mat = {ambient_color, diffuse_color, specular_color, roughness, fresnel, density, reflectivity, refractivity, ior, shininess, type};
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
        
        Triangle triangle = makeTri(Avertice, Bvertice, Cvertice, mat, surf);
        add_Triangle(triangle);
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
    char TP[50];
    
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
        
        Sphere sphere = makeSph(Avertice, bcx, mat, surf);
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
        
        Plane plane = makePln(Avertice, Bvertice, mat);
        add_Plane(plane);
    }
    
    return;
}


void init_Scene_O() {
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
    simd_float3 svec1 = {2, 0.75, -7};
    simd_float3 svec2 = {0, 0.75, -3};
    simd_float3 svec3 = {-2, 0.75, -4};
    add_Object(makeSphere(svec1, 0.75, 0, _images[2]));
    add_Object(makeSphere(svec2, 0.25, 4, NULL));
    add_Object(makeSphere(svec3, 0.25, 4, NULL));
    simd_float3 pvec1 = {0, 0, 0};
    simd_float3 pvec2 = {0, 5, 0};
    simd_float3 pvec3 = {0, 0, -10};
    simd_float3 pvec4 = {4, 0, 0};
    simd_float3 pvec5 = {-4, 0, 0};
    simd_float3 pvec11 = {0, 1, 0};
    simd_float3 pvec12 = {0, -1, 0};
    simd_float3 pvec13 = {0, 0, 1};
    simd_float3 pvec14 = {-1, 0, 0};
    simd_float3 svec15 = {1, 0, 0};
    add_Object(makePlane(pvec1, pvec11, 0));
    add_Object(makePlane(pvec2, pvec12, 0));
    add_Object(makePlane(pvec3, pvec13, 0));
    add_Object(makePlane(pvec4, pvec14, 1));
    add_Object(makePlane(pvec5, svec15, 2));

    simd_float3 t1a = {1, 1, -7};
    simd_float3 t1b = {1, 4.5, -7};
    simd_float3 t1c = {-1, 1, -7};
    
    Object triangle = makeTriangle(t1a, t1b, t1c, 0, _images[4]);
    add_Object(triangle);
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
    load_Spheres("Spheres.txt");
    load_Planes("Planes.txt");
    load_Triangles("Triangles.txt");
}

void init_Scene() {
    #if DRAW_SCENE == 1
    init_Scene1();
    #elif DRAW_SCENE == 2
    init_Scene2();
    #elif DRAW_SCENE == 3
    init_Scene3();
    #elif DRAW_SCENE == 4
    init_Scene4();
    #elif DRAW_SCENE == 5
    init_Scene_OO();
    #else
    nullErrorCheck(NULL, Scene_not_found_error_mssg);
    #endif
}
