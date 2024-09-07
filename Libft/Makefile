NAME := libft.a
FUNC := ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
       ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_strcmp.c \
       ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_strcpy.c \
       ft_calloc.c ft_strdup.c ft_toupper.c ft_tolower.c \
       ft_strchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c \
       ft_strnstr.c ft_atoi.c ft_strrchr.c ft_substr.c \
       ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
       ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
       ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_atoll.c
OBJS := $(FUNC:.c=.o)
CC := cc
CFLAGS := -Wall -Wextra -Werror
AR := ar rcs
RM := rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.SECONDARY: $(OBJS)
.PHONY: all clean fclean re
