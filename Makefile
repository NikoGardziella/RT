# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/01 12:36:10 by pnoutere          #+#    #+#              #
#    Updated: 2022/11/02 09:37:45 by dmalesev         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#COLORS
COLOR := $(shell printf "\e[38;2")
BACKGROUND_COLOR := $(shell printf "\e[48;2")
RESET := $(shell printf "\e[0m")
BOLD := $(shell printf "\e[1m")

#PRINTING TOOLS
ERASE_LINE = \e[K
MOVE = \e[
UP = A
DOWN = B
RIGHT = C
LEFT = D

#FLAGS AND CONFIGS
MAKEFLAGS += --no-print-directory
SHELL = /bin/bash

MAKE_COLOR = ;50;235;150m
PROJECT_NAME = RT
NAME = $(PROJECT_NAME)
PRINT_NAME = $(BOLD)$(COLOR)$(MAKE_COLOR)$(PROJECT_NAME)$(RESET)
CC  = gcc
OPTI_FLAGS = -O3 -flto
DEBUG_FLAGS = -g -fsanitize=address
FLAGS = -Wall -Wextra -Werror -Wconversion
FLAGS += $(DEBUG_FLAGS)
#FLAGS += $(OPTI_FLAGS)

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)
LIBS = $(LIBFT) $(DM_2D) $(DM_VECTORS) $(SDL2)
endif
ifeq ($(UNAME), Linux)
LIBS = $(LIBFT) $(DM_2D) $(DM_VECTORS) $(SDL2) $(MATH_LIBRARY)
endif

MATH_LIBRARY = -lm

LIBRARIES_DIRECTORY = ./libraries

SDL2_CFLAGS = `$(SDL2_BUILD_DIRECTORY)/lib/bin/sdl2-config --cflags`
SDL2_LIBS = `$(SDL2_BUILD_DIRECTORY)/lib/bin/sdl2-config --libs`

SDL2_ARCHIVE = $(LIBRARIES_DIRECTORY)/sdl2.tar.gz
SDL2_BUILD_DIRECTORY = $(LIBRARIES_DIRECTORY)/sdl2
SDL2_DIRECTORY = $(LIBRARIES_DIRECTORY)/libsdl2
SDL2 = $(SDL2_BUILD_DIRECTORY)/lib/lib/libSDL2.a
SDL2_HEADERS = $(SDL2_BUILD_DIRECTORY)/lib/include/SDL2

DM_VECTORS_DIRECTORY = $(LIBRARIES_DIRECTORY)/dm_vectors
DM_VECTORS = $(DM_VECTORS_DIRECTORY)/dm_vectors.a
DM_VECTORS_HEADERS = $(DM_VECTORS_DIRECTORY)/includes

DM_2D_DIRECTORY = $(LIBRARIES_DIRECTORY)/dm_2d
DM_2D = $(DM_2D_DIRECTORY)/dm_2d.a
DM_2D_HEADERS = $(DM_2D_DIRECTORY)/includes

LIBFT_DIRECTORY = $(LIBRARIES_DIRECTORY)/libft
LIBFT = $(LIBFT_DIRECTORY)/libft.a
LIBFT_HEADERS = $(LIBFT_DIRECTORY)/includes

HEADERS_DIRECTORY = ./includes/
HEADERS_LIST =	rt.h
HEADERS = $(addprefix $(HEADERS_DIRECTORY), $(HEADERS_LIST))

SOURCES_DIRECTORY = ./sources/
SOURCES_LIST =	main.c\
				init/create_images.c\
				init/rotation_matrices.c\
				matrix/transform.c\
				images/put_pixel.c\
				images/get_camera_ray.c\
				images/main_image.c\
				images/process_image.c\
				images/sidebar_button.c\
				images/ray_debugger.c\
				images/render_scene.c\
				parser/add_object_to_scene.c\
				parser/load_scene_camera.c\
				parser/load_scene_objects.c\
				parser/read_camera_info.c\
				parser/read_object_info.c\
				parser/read_object_transformations.c\
				intersects.c
SOURCES = $(addprefix $(SOURCES_DIRECTORY), $(SOURCES_LIST))
SOURCE_COUNT = $(words $(SOURCES_LIST))

OBJECTS_DIRECTORY = objects/
OBJECTS_LIST = $(patsubst %.c, %.o, $(SOURCES_LIST))
OBJECTS	= $(addprefix $(OBJECTS_DIRECTORY), $(OBJECTS_LIST))

INCLUDES = -I$(HEADERS_DIRECTORY) -I$(LIBFT_HEADERS) -I$(SDL2_HEADERS) -I$(DM_2D_HEADERS) -I$(DM_VECTORS_HEADERS)

ASSERT_OBJECT = && printf "$(ERASE_LINE)" && printf "$@ $(COLOR)$(MAKE_COLOR)$(BOLD) ✓$(RESET)" || printf "$@ $(COLOR)$(MAKE_COLOR)$(BOLD)✘$(RESET)\n\n" | exit -1

ifneq ($(MAKECMDGOALS),progress_bar)
$(info Entering $(PRINT_NAME) Makefile!)
endif

all: $(NAME)

$(NAME): $(SDL2) $(LIBFT) $(DM_2D) $(DM_VECTORS) $(OBJECTS_DIRECTORY) $(OBJECTS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJECTS) $(SDL2_LIBS) $(SDL2_CFLAGS) $(LIBS) -o $(NAME)
	@printf "Compiled $(BOLD)$(COLOR)$(MAKE_COLOR)$(NAME)$(RESET)!\n\n"

$(OBJECTS_DIRECTORY):
	@mkdir -p $(OBJECTS_DIRECTORY)
	@mkdir -p $(OBJECTS_DIRECTORY)/init
	@mkdir -p $(OBJECTS_DIRECTORY)/parser
	@mkdir -p $(OBJECTS_DIRECTORY)/images
	@mkdir -p $(OBJECTS_DIRECTORY)/matrix
	@printf "$(COLOR)$(MAKE_COLOR)__________________________________________________________________________________\n"
	@printf "$(PRINT_NAME): Created $(OBJECTS_DIRECTORY) directory.$(RESET)\n\n\n"

$(OBJECTS_DIRECTORY)%.o : $(SOURCES_DIRECTORY)%.c $(HEADERS)
	@printf "$(MOVE)2$(UP)"
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@ $(ASSERT_OBJECT)
	@make progress_bar

$(SDL2): $(SDL2_DIRECTORY) $(SDL2_BUILD_DIRECTORY)
	@cd $(SDL2_BUILD_DIRECTORY) && ../libsdl2/configure --prefix=`pwd`/lib
	@make -C $(SDL2_BUILD_DIRECTORY)
	@make -C $(SDL2_BUILD_DIRECTORY) install

$(SDL2_BUILD_DIRECTORY):
	@mkdir $(SDL2_BUILD_DIRECTORY)

$(SDL2_DIRECTORY):
	@printf "Unpacking $(SDL2_ARCHIVE)\n\n"
	@tar -xf $(SDL2_ARCHIVE) -C $(LIBRARIES_DIRECTORY)
	@mv $(LIBRARIES_DIRECTORY)/SDL $(SDL2_DIRECTORY)

$(LIBFT):
	@make -C $(LIBFT_DIRECTORY)

$(DM_VECTORS):
	@make -C $(DM_VECTORS_DIRECTORY)

$(DM_2D):
	@make -C $(DM_2D_DIRECTORY)

clean:
	@rm -rf $(OBJECTS_DIRECTORY)
	@printf "$(PRINT_NAME): Deleted $(OBJECTS_DIRECTORY)*\n"
	@make -C $(LIBFT_DIRECTORY) clean
	@make -C $(DM_2D_DIRECTORY) clean
	@make -C $(DM_VECTORS_DIRECTORY) clean
	@printf "\n"

bclean:
	@rm -f $(NAME)
	@printf "$(PRINT_NAME): Deleted $(NAME)\n"

fclean: clean
	@make bclean
	@make -C $(LIBFT_DIRECTORY) bclean
	@make -C $(DM_2D_DIRECTORY) bclean
	@make -C $(DM_VECTORS_DIRECTORY) bclean
	@rm -rf $(SDL2_BUILD_DIRECTORY)
	@rm -rf $(SDL2_DIRECTORY)
	@printf "\n"

re: fclean all

progress_bar:
	$(eval LOADED_COUNT = $(words $(shell find $(OBJECTS_DIRECTORY) -name '*.o')))
	@printf "\r$(MOVE)76$(RIGHT)Files compiled [$(BOLD)$(COLOR)$(MAKE_COLOR)$(LOADED_COUNT)$(RESET) / $(BOLD)$(COLOR)$(MAKE_COLOR)$(SOURCE_COUNT)$(RESET)]\n"
	@for ((i = 1; i <= 100; i++)) ; do\
		if ((i <= $(LOADED_COUNT) * 100 / $(SOURCE_COUNT))) ; then \
			printf "$(BACKGROUND_COLOR)$(MAKE_COLOR)$(COLOR);0;0;0m" ; \
		else \
			printf "$(BACKGROUND_COLOR);255;255;255m$(COLOR);0;0;0m" ; \
		fi ; \
		if ((i == 47 && $(LOADED_COUNT) * 100 / $(SOURCE_COUNT) >= 100)) ; then \
			printf "[$(RESET)" ; \
		elif ((i == 48 && $(LOADED_COUNT) * 100 / $(SOURCE_COUNT) >= 10 && $(LOADED_COUNT) * 100 / $(SOURCE_COUNT) < 100)) ; then \
			printf "[$(RESET)" ; \
		elif ((i == 49 && $(LOADED_COUNT) * 100 / $(SOURCE_COUNT) < 10)) ; then \
			printf "[$(RESET)" ; \
		elif ((i == 48 && $(LOADED_COUNT) * 100 / $(SOURCE_COUNT) >= 100)) ; then \
			printf "$$(($(LOADED_COUNT) * 100 / $(SOURCE_COUNT) / 100 % 10))$(RESET)" ; \
		elif ((i == 49 && $(LOADED_COUNT) * 100 / $(SOURCE_COUNT) >= 10)) ; then \
			printf "$$(($(LOADED_COUNT) * 100 / $(SOURCE_COUNT) / 10 % 10))$(RESET)" ; \
		elif ((i == 50)) ; then \
			printf "$$(($(LOADED_COUNT) * 100 / $(SOURCE_COUNT) % 10))$(RESET)" ; \
		elif ((i == 51)) ; then \
			printf "." ; \
		elif ((i == 52)) ; then \
			printf "$$(($(LOADED_COUNT) * 1000 / $(SOURCE_COUNT) % 10))$(RESET)" ; \
		elif ((i == 53)) ; then \
			printf "%%$(RESET)" ; \
		elif ((i == 54)) ; then \
			printf "]$(RESET)" ; \
		else \
			printf " $(RESET)"; \
		fi ; \
	done ;
	@printf "\n"

.PHONY: all clean fclean re
