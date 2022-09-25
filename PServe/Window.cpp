#include "Window.h"
Window::Window(RenderSettings settings, const char* saveFile) {
    this->settings = settings;
    this->saveFile = saveFile;
    image = new uint8_t[settings.width * settings.height * 3];
    texUploaded = false;
    texID = NULL;
    windowPtr = nullptr;
    finishedRendering = new std::atomic_bool(false);
    finishedDenoising = new std::atomic_bool(false);
}

int Window::createWindow() {

    if (!glfwInit()) return 1;
    windowPtr = glfwCreateWindow(settings.width, settings.height, "PServe", NULL, NULL);
    if (!windowPtr)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(windowPtr);

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
    ImGui_ImplOpenGL2_Init();

    return 0;
}

void Window::uploadImage(bool flip) {
    uint8_t* texture = nullptr;
    if (flip) {
        texture = new uint8_t[settings.width * settings.height * 3];
        for (unsigned int y = 0; y < settings.height; y++) {
            for (unsigned int x = 0; x < settings.width; x++) {
                int newIndex = 3 * (x + (settings.height - 1 - y) * settings.width);
                texture[newIndex + 0] = image[3 * (x + y * settings.width) + 0];
                texture[newIndex + 1] = image[3 * (x + y * settings.width) + 1];
                texture[newIndex + 2] = image[3 * (x + y * settings.width) + 2];
            }
        }
    }

    texUploaded = true;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, settings.width, settings.height, 0, GL_RGB, GL_UNSIGNED_BYTE, flip ? texture : image);

    if (flip) delete[](texture);
}



void Window::begin() {
    while (!glfwWindowShouldClose(windowPtr))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("PServe Ctonrols");

        if (finishedRendering->load()) {
            std::cout << "joining" << std::endl;
            renderThread.join();
            finishedRendering->store(false);
            renderThread.~thread();

            imageToColor(settings.width, settings.height, image, settings.framebuffer->color);
            uploadImage(true);
        }

        if (finishedDenoising->load()) {
            std::cout << "joining" << std::endl;
            renderThread.join();
            finishedDenoising->store(false);
            renderThread.~thread();

            imageToColor(settings.width, settings.height, image, settings.beauty);
            uploadImage(true);
        }

        if (ImGui::Button("Render") && !finishedRendering->load()) {
            finishedDenoising->store(false);
            std::cout << "rendering an image: " << settings.width << ", " << settings.height << std::endl;
            renderThread = std::thread(render, settings, finishedRendering);
        }
            
        /*if (ImGui::Button("Denoise") && !finishedRendering->load() && texUploaded) {
            renderThread = std::thread(denoise, settings, finishedDenoising);
            uploadImage(true);
        }*/

        ImGui::Checkbox("Save All", &saveAll);

        if (ImGui::Button("Save Image")) {
            if (saveAll) {
                uint8_t* tempImage = (uint8_t*)malloc(settings.width * settings.height * 3 * sizeof(uint8_t));
                
                saveImage("", std::string(saveFile), settings.width, settings.height, image);

                std::cout << "writing images" << std::endl;

                //albedo
                imageToColor(settings.width, settings.height, image, settings.framebuffer->albedo);
                saveImage("", std::string(saveFile) + "-albedo", settings.width, settings.height, tempImage);

                //normal
                imageToColor(settings.width, settings.height, image, settings.framebuffer->normal, true);
                saveImage("", std::string(saveFile) + "-normal", settings.width, settings.height, tempImage);

                //noisy
                imageToColor(settings.width, settings.height, image, settings.framebuffer->color);
                saveImage("", std::string(saveFile) + "-noisy", settings.width, settings.height, tempImage);


                //denoised
                saveImage("", std::string(saveFile), settings.width, settings.height, this->image);
            }
            else {
                saveImage("", std::string(saveFile), settings.width, settings.height, image);
            }
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();


        glClear(GL_COLOR_BUFFER_BIT);

        //If texture uploaded, draw to screen
        if (texUploaded) {
            glBegin(GL_TRIANGLE_STRIP);

            glTexCoord2f(0.0, 0.0);
            glVertex2f(-1.0, -1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex2f(1.0, -1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex2f(-1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex2f(1.0, 1.0);


            glEnd();
        }
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(windowPtr);
        glfwSwapBuffers(windowPtr);
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    glDeleteTextures(1, &texID);
}