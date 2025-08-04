NAME		= miniRT
OBJDIR		= .obj
SRCS		= srcs/main.c srcs/init.c srcs/vector.c srcs/cli.c \
			  srcs/render/render_test.c srcs/render/render.c srcs/render/ray_trace.c \
			  srcs/render/render_frame.c \
			  srcs/objects/plane.c srcs/objects/sphere.c \
			  srcs/shading/diffuse_lights.c
OBJS		= $(SRCS:%.c=$(OBJDIR)/%.o)



LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

MLX_DIR		= minilibx-linux
MLX			= $(MLX_DIR)/libmlx.a



CC			= cc
# CFLAGS	= -Wall -Wextra -Werror -fsanitize=address -g 
CFLAGS		= -Wall -Wextra -Werror -g 
# CFLAGS		= -g
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
