# ----------------------------
# Makefile Options
# ----------------------------

NAME = PROGRESS
ICON = icon.png
DESCRIPTION = "A clone of ProgressQuest for the TI-84+CE"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
