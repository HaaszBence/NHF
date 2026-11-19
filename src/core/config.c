#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static Config cfg;

const Config *config_get_defaults(void)
{
    cfg = (Config){
        .screen_width = DEF_SCREEN_WIDTH,
        .screen_height = DEF_SCREEN_HEIGHT,
        .target_fps = DEF_FPS,
        .fullscreen = DEF_FULLSCREEN,
        .master_volume = DEF_MASTER_VOLUME,
        .title = DEF_TITLE
    };
    return &cfg;
}

bool config_save(const Config *c)
{
    FILE *f = fopen(CONFIG_PATH, "w");
    if (!f) return false;

    fprintf(f,
    "screen_width=%d\n"
    "screen_height=%d\n"
    "target_fps=%d\n"
    "fullscreen=%d\n"
    "master_volume=%.2f\n"
    "title=%s\n",

    c->screen_width,
    c->screen_height,
    c->target_fps,
    c->fullscreen,
    c->master_volume,
    c->title
    );

    fclose(f);
    return true;
}

const Config *config_init(void)
{
    config_get_defaults(); // Initialize with default values

    // Attempt to open the configuration file for reading
    FILE *f = fopen(CONFIG_PATH, "r");
    if (!f) {
        config_save(&cfg); // If the file doesn't exist, save the default configuration
        return &cfg;
    }

    int count = 0; // Count of successfully read configuration fields
    char line[512]; // Buffer to hold each line read from the configuration file
    while (fgets(line, sizeof(line), f)) // Read into the buffer until a newline or EOF is encountered
    {
        line[strcspn(line, "\r\n")] = '\0'; // Remove any trailing newline characters from the line
        char *eq = strchr(line, '='); // Find the position of the '=' character in the line
        if (!eq) continue; // If no '=' is found, skip this line as it's not a valid key-value pair
        *eq = '\0'; // Split the line into key and value by replacing '=' with a null terminator
        const char *k = line, *v = eq + 1; // Set k to point to the key and v to point to the value

        // Compare the key with known configuration fields and assign the corresponding value to the cfg structure
        if (!strcmp(k, "screen_width")) { cfg.screen_width = atoi(v); count++; }
        else if (!strcmp(k, "screen_height")) { cfg.screen_height = atoi(v); count++; }
        else if (!strcmp(k, "target_fps")) { cfg.target_fps = atoi(v); count++; }
        else if (!strcmp(k, "fullscreen")) { cfg.fullscreen = atoi(v); count++; }
        else if (!strcmp(k, "master_volume")) { cfg.master_volume = (float)atof(v); count++; }
        else if (!strcmp(k, "title")) { snprintf(cfg.title, sizeof(cfg.title), "%s", v); count++; }
    }
    fclose(f);

    // If the configuration file is missing any required fields or has invalid values, reset to defaults and save.
    if (count != 6 || cfg.screen_width < 640 || cfg.screen_height < 360 || cfg.target_fps < 15) {
        config_get_defaults();
        config_save(&cfg);
    }

    return &cfg;
}