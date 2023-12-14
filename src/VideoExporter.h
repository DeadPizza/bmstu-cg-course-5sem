#pragma once

#include <cstdio>
#include <glad/glad.h>
#include <format>
#include <filesystem>

class VideoExporter
{
public:
    VideoExporter(int framerate)
    {
        int video_num = 0;
        while (std::filesystem::exists(std::format("out-{}.mp4", video_num))){++video_num;}
        auto command = std::format("ffmpeg -y -f rawvideo -s {}x{} -pix_fmt rgb24 -r {} -i - -vf vflip -an -b:v 5000k out-{}.mp4", 1280, 720, framerate, video_num);
        m_file = popen(command.c_str(), "w");
        cur_pixels = new char[1280 * 720 * 3];
    }

    ~VideoExporter()
    {
        delete cur_pixels;
        fclose(m_file);
    }

    void CaptureFrame()
    {
        glReadPixels(0, 0, 1280, 720, GL_RGB, GL_UNSIGNED_BYTE, cur_pixels);
        fwrite(cur_pixels, 1280 * 720 * 3, 1, m_file);
    }

private:
    FILE *m_file;
    char *cur_pixels;
};