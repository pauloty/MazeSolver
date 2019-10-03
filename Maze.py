import turtle

# Initial params for drawing
wn = turtle.Screen()
wn.bgcolor("black")
wn.title("Maze")
wn.setup(700, 700)


# Pen classes used for drawing. They have different speeds for seeing the drawing of the path
class Pen(turtle.Turtle):
    def __init__(self):
        turtle.Turtle.__init__(self)
        self.shape("square")
        self.penup()
        self.speed(0)


class PenPath(turtle.Turtle):
    def __init__(self):
        turtle.Turtle.__init__(self)
        self.shape("square")
        self.penup()
        self.speed(1)


# Function used to read the maze from a file
def read_maze():
    maze = list()
    with open("maze.txt") as f:
        header = f.readline()
        for line in f:
            maze.append(list(line.rstrip()))
    # dimensions[0]: height / dimensions[1]: width
    dimensions = header.split(' ')
    return dimensions, maze


# Function used to find the start of the maze
def find_start(maze):
    for i in range(len(maze)):
        for j in range(len(maze[0])):
            if maze[i][j] == '#':
                return tuple([i, j])
    return None


# Function used to find the end of the maze
def find_end(maze):
    for i in range(len(maze)):
        for j in range(len(maze[0])):
            if maze[i][j] == '$':
                return tuple([i, j])
    return None


# Function used to find the children of a given node
def get_children(maze, root, visited, dimensions):

    # List of possible movements
    movements = list()
    movements.append((root[0]-1, root[1]))    # Up
    movements.append((root[0]-1, root[1]+1))  # Up/Right
    movements.append((root[0], root[1]+1))    # Right
    movements.append((root[0]+1, root[1]+1))  # Down/Right
    movements.append((root[0]+1, root[1]))    # Down
    movements.append((root[0]+1, root[1]-1))  # Down/Left
    movements.append((root[0], root[1]-1))    # Left
    movements.append((root[0]-1, root[1]-1))  # Up/Left

    # Restrictions to be a valid movement
    children = list()
    for i in movements:
        if 0 <= i[0] < int(dimensions[0]) and 0 <= i[1] < int(dimensions[1]):
            if maze[i[0]][i[1]] != '-' and i not in visited:
                children.append(i)
    return children


# Function used to find the path using BFS algorithm
def bfs(maze, dimensions, start, end):
    queue = [[start]]
    visited = set()

    while len(queue) != 0:
        path = queue.pop(0)
        current = path[-1]
        if current == end:
            return path
        elif current not in visited:
            for child in get_children(maze, current, visited, dimensions):
                # Construction of the path for each children
                newpath = list(path)
                newpath.append(child)

                queue.append(newpath)
            visited.add(current)
    return None


# Function used to draw the maze
def setup_maze(maze, pen):
    for y in range(len(maze)):
        for x in range(len(maze[0])):
            character = maze[y][x]
            screen_x = -288 + (x * 24)
            screen_y = 288 - (y * 24)

            # Draws the obstacles
            if character == "-":
                pen.color("gray")
                pen.goto(screen_x, screen_y)
                pen.stamp()
            # Draws the valid positions
            elif character == "*":
                pen.color("white")
                pen.goto(screen_x, screen_y)
                pen.stamp()
            # Draws the start of the maze
            elif character == "#":
                pen.color("green")
                pen.goto(screen_x, screen_y)
                pen.stamp()
            # Draws the end of the maze
            elif character == "$":
                pen.color("red")
                pen.goto(screen_x, screen_y)
                pen.stamp()


# Draws the path found in the maze
def print_path(path, pen):
    # Skip the start and the end
    path = path[1:-1]
    pen.color("yellow")
    for pos in path:
        y = pos[0]
        x = pos[1]
        screen_x = -288 + (x * 24)
        screen_y = 288 - (y * 24)
        pen.goto(screen_x, screen_y)
        pen.stamp()


def main():
    dimensions, maze = read_maze()
    start = find_start(maze)
    end = find_end(maze)
    path = bfs(maze, dimensions, start, end)

    pen = Pen()
    penpath = PenPath()
    setup_maze(maze, pen)
    print_path(path, penpath)

    wn.tracer(0)
    turtle.mainloop()


if __name__ == '__main__':
    main()