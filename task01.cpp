#include <glad/glad.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl3.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include "ramen/ramen.h"
#include "ramen/rgl_camera.h"
#include "ramen/rgl_defines.h"
#include "ramen/rgl_math.h"
#include "ramen/rgl_model.h"
#include "ramen/rgl_shader.h"

static std::vector<Vertex> g_Cone;
static std::vector<Vertex> g_Cap;
static GLuint              g_ConeVBO;
static GLuint              g_CapVBO;

void CreateGeometry()
{
    // Triangle Fan mit 18 Vertices anlegen
    // Die Spitze des Konus ist ein Vertex, den alle Triangles gemeinsam haben;
    // um einen Konus anstatt einen Kreis zu produzieren muss der Vertex einen positiven z-Wert haben
    g_Cone.push_back(Vertex{ .position = Vec3f{ 0.0f, 0.0f, 75.0f }, .normal = Vec3f{}, .color = Vec3f{ 0, 1, 0 } });
    // Kreise um den Mittelpunkt und spezifiziere Vertices entlang des Kreises
    // um einen Triangle_Fan zu erzeugen
    int iPivot = 1;
    for ( float angle = 0.0f; angle < (2.0f * RAMEN_PI); angle += (RAMEN_PI / 8.0f) )
    {
        // Berechne x und y Positionen des naechsten Vertex
        float x = 50.0f * sinf(angle);
        float y = 50.0f * cosf(angle);

        Vertex v{};

        // Alterniere die Farbe
        if ( (iPivot % 2) == 0 )
            v.color = Vec3f{ 0.235f, 0.235f, 0.235f };
        else
            v.color = Vec3f{ 0.0f, 0.6f, 1.0f };

        // Inkrementiere iPivot um die Farbe beim naechsten mal zu wechseln
        iPivot++;

        // Spezifiziere den naechsten Vertex des Triangle_Fans
        v.position = Vec3f{ x, y, 0.0f };
        g_Cone.push_back(v);
    }

    // Fertig mit dem Konus
    /*
    // Erzeuge einen weiteren Triangle_Fan um den Boden zu bedecken
    boden.Begin(GL_TRIANGLE_FAN, 18);
    // Das Zentrum des Triangle_Fans ist im Ursprung
    boden.Vertex3f(0.0f, 0.0f, 0.0f);
    for ( float angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f) )
    {
        // Berechne x und y Positionen des naechsten Vertex
        float x = 50.0f * sin(angle);
        float y = 50.0f * cos(angle);

        // Alterniere die Farbe
        if ( (iPivot % 2) == 0 )
            boden.Color4f(1, 0.8, 0.2, 1);
        else
            boden.Color4f(0, 0.8, 0, 1);

        // Inkrementiere iPivot um die Farbe beim naechsten mal zu wechseln
        iPivot++;

        // Spezifiziere den naechsten Vertex des Triangle_Fans
        boden.Vertex3f(x, y, 0);
    }

    // Fertig mit dem Bodens
    boden.End();
    */

    glCreateBuffers(1, &g_ConeVBO);
    glNamedBufferData(g_ConeVBO, g_Cone.size() * sizeof(Vertex), g_Cone.data(), GL_STATIC_DRAW);
}

int main(int argc, char** argv)
{
    Ramen* pRamen = Ramen::Create();
    pRamen->Init("Task 02", 800, 600);

    /* Load shaders. */
    Shader shader{};
    if ( !shader.Load("./shaders/task01.vert", "./shaders/task01.frag") )
    {
        fprintf(stderr, "Could not load shader.\n");
    }

    /* Create a model on GPU */
    Model model{};
    if ( !model.Load("./models/stormtrooper.obj") )
    {
        fprintf(stderr, "Could not load model file.\n");
    }

    /* Create Cylinder and upload to GPU */
    CreateGeometry();

    /* Create camera */
    Camera camera(Vec3f{ 0.0f, 0.0f, 30.0f }, Vec3f{ 0.0f, 0.0f, 0.0f });
    camera.RotateAroundUp(-20.0f);

    /* Model mat*/
    Mat4f modelMat = Scale(Vec3f{ 3.0f, 3.0f, 3.0f });
    Translate(modelMat, Vec3f{ 0.0f, 0.0f, -20.0f }); //Mat4f::Identity();

    /* VAO. */
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    glVertexArrayVertexBuffer(VAO, 0, model.GetBuffer(), 0, sizeof(Vertex));
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    /* Some global GL states */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    // glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    /* Main loop */
    bool isRunning = true;
    SDL_GL_SetSwapInterval(1); /* 1 = VSync enabled; 0 = VSync disabled */
    glPointSize(20.0f);        // TODO: Delete later.
    while ( isRunning )
    {
        SDL_Event e;
        while ( SDL_PollEvent(&e) )
        {
            ImGui_ImplSDL3_ProcessEvent(&e);

            if ( e.type == SDL_EVENT_QUIT )
            {
                isRunning = false;
            }

            if ( e.type == SDL_EVENT_KEY_DOWN )
            {
                switch ( e.key.key )
                {
                case SDLK_ESCAPE:
                {
                    isRunning = false;
                }
                break;

                default:
                {
                }
                }
            }
        }

        /* Query new frame dimensions */
        int windowWidth, windowHeight;
        SDL_GetWindowSize(pRamen->GetWindow(), &windowWidth, &windowHeight);

        /* Adjust viewport and perspective projection accordingly. */
        glViewport(0, 0, windowWidth, windowHeight);

        /* View mat */
        Mat4f viewMat = LookAt(
            camera.GetPosition(), camera.GetPosition() + camera.GetForward(), camera.GetUp()); // Mat4f::Identity();

        /* Projection mat */
        float aspect  = (float)windowWidth / (float)windowHeight;
        Mat4f projMat = PerspectiveProjection(TO_RAD(70.0f), aspect, 0.01f, 500.0f);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        /* ImGUI Rendering */
        ImGui::Render();

        /* Rendering */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 0.95f, 0.0f, 1.0f);

        shader.Use();
        glBindVertexArray(VAO);
        glUniformMatrix4fv(0, 1, GL_FALSE, modelMat.Data());
        glUniformMatrix4fv(1, 1, GL_FALSE, viewMat.Data());
        glUniformMatrix4fv(2, 1, GL_FALSE, projMat.Data());
        glDrawArrays(GL_TRIANGLES, 0, model.NumVertices());

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(pRamen->GetWindow());
    }

    /* GL Resources shutdown. */
    shader.Delete();
    glDeleteVertexArrays(1, &VAO);

    /* Ramen Shutdown */
    pRamen->Shutdown();

    return 0;
}
