#define _GNU_SOURCE
#include "renderer.h"

#include <gtk/gtk.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROJECT_NAME "dankeyboard"
#define PROJECT_ID   "null.daknig.dankeyboard"

static void cb_activate(GtkApplication *app, gpointer _);

typedef enum ButtonAction {
	BUTTON_ACTION_INPUT_TEXT,
	BUTTON_ACTION_SWITCH_LAYOUT
} ButtonAction;
typedef struct Button {
	ButtonAction type;
	union {
		char *text; // valid when INPUT_TEXT
		size_t dest_index; // valid when SWITCH_LAYOUT
	};
} Button;

#define MAX_ROWS            7
#define MAX_BUTTONS_PER_ROW 250
#define MAX_LAYOUTS         10

uint8_t layout_count = 0;
uint8_t layout_index = 0;
struct Layout {
	uint8_t row_count;
	struct Row {
		uint8_t button_count;
		Button buttons[MAX_BUTTONS_PER_ROW]; // as many as you want basically
	} rows[MAX_ROWS]; // max of 7 rows seems fair :)
} layouts[10];

int main(int argc, char **argv) {
	GtkApplication *app;
	app = gtk_application_new(PROJECT_ID, G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(cb_activate), NULL);

	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}

static void cb_activate(GtkApplication *app, gpointer _) {
	(void)_;
	(void)app;

	// now lets read some configs!

	char *config_dir;
	int config_dir_len = asprintf(&config_dir, "%s/%s",
	                              g_get_user_config_dir(),
	                              PROJECT_NAME);
	if (config_dir_len < 0) {
		/* fprintf(stderr, "malloc."); */
		g_error("malloc."); // kinda terminates.
		__builtin_unreachable();
	}
	puts(config_dir);

	char *config_fname;
	int config_fname_len = asprintf(&config_fname, "%s/config", config_dir);
	if (config_fname_len < 0) {
		g_error("malloc.");
		__builtin_unreachable();
	}

	FILE *config;
	if (NULL == (config = fopen(config_fname, "r"))) {
		g_warning("cant open config");
		/* fprintf(stderr, "cant open config"); */
		// create file
		if (NULL != (config = fopen(config_fname, "w"))) {
			// make a default simple qwerty layout
			fprintf(config, "q w e r t y u i o p\n"
			                " a s d f g h j k l\n"
			                "   z x c v b n m\n"
			                "\n");
			// close and continue
			fclose(config);
			config = fopen(config_fname, "r");
		}
		if (NULL == config) { // could not create file
			/* g_error */ fprintf(stderr,
			                      "could not create config file at %s!",
			                      config_fname);
			__builtin_unreachable();
		}
	}

	// read file into memory
	char *line = NULL;
	size_t line_len = 0;
	layouts[0].row_count = 0;
	for (int row_i = 0; row_i < MAX_ROWS; row_i++) {
		if (feof(config)) break;
		getline(&line, &line_len, config);
		// strip trailing whitespace
		for (char *end = &line[strlen(line) - 1];
		     *line && strchr("\n \t", *end); end--) {

			*end = '\0';
		}

		if (*line == '\0') // empty line
			break; // should be actually: layout++
		layouts[0].row_count = row_i + 1;

		struct Row row; /* &layouts[0].rows[row_i]; */
		row.button_count = 0;

		char *saveptr = line;
		for (int button_i = 0;
		     saveptr && *saveptr && button_i < MAX_BUTTONS_PER_ROW;
		     button_i++) {

			saveptr += strspn(saveptr, " "); // go past the whitespace

			// for worst case... alloc for the whole line
			char* button_text = calloc(1, strlen(saveptr));

			for (char *p = button_text; *saveptr && *saveptr != ' ';
			     p++, saveptr++) {
				if (*saveptr == '\\') { // escape character
					saveptr++;
					switch (*saveptr) {
					case 'n': *p = '\n'; break;
					case 't': *p = '\t'; break;
					case 'r': *p = '\r'; break;
					case '\\': *p = '\\'; break;
					case ' ': *p = ' '; break;
					default:
						*p++ = '\\';
						*p = *saveptr;
						break;// do nothing
					}
					continue;
				} else {
					*p = *saveptr;
				}
			}
			// save space
			button_text = realloc(button_text, strlen(button_text));
			if (NULL == button_text) {
				g_error("malloc.");
				__builtin_unreachable();
			}

			row.button_count = button_i + 1;
			row.buttons[button_i]
			    = (Button){.type = BUTTON_ACTION_INPUT_TEXT,
			               .text = button_text};
			/* strcspn(saveptr, " \n"); */
		}
		layouts[0].rows[row_i] = row;
	}

	fclose(config);

	// show what you learned!
	for (int row_i = 0; row_i < layouts[0].row_count; row_i++) {
		struct Row *row = &layouts[0].rows[row_i];
		for (int button_i = 0; button_i < row->button_count; button_i++) {
			printf("%s ", row->buttons[button_i].text);
		}
		puts("");
	}
	/*

	GtkWidget *window = gtk_window_new();
	gtk_window_set_default_size(GTK_WINDOW(window), 360, 294);
	gtk_window_set_application(GTK_WINDOW(window), app);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
	gtk_widget_set_hexpand(box, TRUE);
	gtk_widget_set_vexpand(box, TRUE);
	gtk_window_set_child(GTK_WINDOW(window), box);

	GtkWidget *label = gtk_label_new("hello world!");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_widget_set_parent(label, box);

	gtk_window_present(GTK_WINDOW(window));
	*/
}
