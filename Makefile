NAME		= miniRT
OBJDIR		= .obj
SRCS		= srcs/main.c srcs/init.c srcs/vector.c srcs/cli.c srcs/debug.c \
			  srcs/parser/parser.c srcs/parser/shape_builder.c srcs/parser/assigners.c srcs/parser/materials.c srcs/parser/set_builder.c \
			  srcs/render/render_test.c srcs/render/render.c srcs/render/ray_trace.c \
			  srcs/render/render_frame.c srcs/cleaner.c \
			  srcs/objects/plane.c srcs/objects/sphere.c srcs/objects/cylinder.c \
			  srcs/objects/cone.c srcs/objects/triangle.c \
			  srcs/shading/diffuse_lights.c srcs/shading/shading.c srcs/shading/specular_highlight.c \
			  srcs/shading/reflection.c srcs/shading/refraction.c srcs/shading/shadows.c \
			  srcs/shading/shadow_utils.c srcs/shading/shading_utils.c srcs/shading/shading_texture.c\
			  srcs/texture/texture.c srcs/texture/uv_cylinder.c srcs/texture/uv_cone.c \
			  srcs/texture/uv_triangle.c srcs/texture/uv_map.c srcs/texture/image_sample.c \
			  srcs/texture/image_load.c srcs/texture/skybox.c \
			  srcs/texture/bump_map.c srcs/texture/bump_map_t_tb.c srcs/texture/bump_map_utils.c \
			  srcs/texture/bump_build.c
OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS		= $(SRCS:%.c=$(OBJDIR)/%.d)



LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

MLX_DIR		= minilibx-linux
MLX			= $(MLX_DIR)/libmlx.a



CC			= cc
# CFLAGS	= -Wall -Wextra -Werror -fsanitize=address -g 
# CFLAGS		= -Wall -Wextra -Werror -g 
CFLAGS		= -g -MMD
INCLUDES	= -I./includes -I$(LIBFT_DIR) -I$(MLX_DIR) -Ilibft/includes
LDFLAGS		= -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lX11 -lXext -lm

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS)
	@echo ">>> Linking $(NAME)"
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJDIR)/%.o: %.c
	@echo ">>> Compiling $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

ftbranch:
	@echo ">>> Checking out libft main branch"
	@git -C $(LIBFT_DIR) checkout main

$(LIBFT): ftbranch
	@echo ">>> Building libft"
	make -C $(LIBFT_DIR)


$(MLX_DIR):
	@echo ">>> Cloning MiniLibX"
	git clone https://github.com/42Paris/minilibx-linux.git $(MLX_DIR)

$(MLX): | $(MLX_DIR)
	@echo ">>> Building MiniLibX"
	make -C $(MLX_DIR)



clean:
	@echo ">>> Cleaning all object files and libraries"
	make clean -C $(LIBFT_DIR)
	make clean -C $(MLX_DIR)
	rm -rf $(OBJDIR)

fclean: clean
	@echo ">>> Full clean (including binary)"
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

relibft:
	@echo ">>> Rebuilding libft"
	make re -C $(LIBFT_DIR)

re: ftbranch fclean relibft all

-include $(OBJS:.o=.d)
