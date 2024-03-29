import turtle
import re
from ast import literal_eval

# Pen classes used for drawing. They have different speeds for seeing the drawing of the path
class Pen(turtle.Turtle):
    def __init__(self, square_size):
        turtle.Turtle.__init__(self)
        self.shape("square")
        self.shapesize(square_size/24)
        self.penup()
        self.speed(0)


class PenPath(turtle.Turtle):
    def __init__(self, square_size):
        turtle.Turtle.__init__(self)
        self.shape("square")
        self.shapesize(square_size / 24)
        self.penup()
        self.speed(1)


# Function used to read the maze from a file
def read_maze(maze_num):
    maze = list()
    with open("labirintos/labirinto" + str(int(maze_num)) + ".txt") as f:
        header = f.readline()
        for line in f:
            maze.append(list(line.rstrip()))
    # dimensions[0]: height / dimensions[1]: width
    dimensions = header.split(' ')
    return dimensions, maze


# Function used to draw the maze
def setup_maze(maze, pen):
    # Starting point for drawing
    p = 650 / len(maze)
    if len(maze) % 2 == 0:
        start_pos = (len(maze) / 2) * p
    else:
        start_pos = ((len(maze) - 1) / 2) * p

    for y in range(len(maze)):
        for x in range(len(maze[0])):
            character = maze[y][x]
            screen_x = -start_pos + (x * p)
            screen_y = start_pos - (y * p)

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
def print_path(path, pen, maze):   
    turtle.delay(9)
    if not path:
        t = turtle.Turtle(visible=False)
        t.penup()
        t.color("red")
        t.write("Caminho não encontrado", align="center", font=("Arial", 30, "bold"))
    else:
        # Starting point for drawing
        p = 650 / len(maze)
        if len(maze) % 2 == 0:
            start_pos = (len(maze) / 2) * p
        else:
            start_pos = ((len(maze) - 1) / 2) * p

        # Skip the start and the end
        path = path[1:-1]
        pen.color("yellow")
        for pos in path:
            y = pos[0]
            x = pos[1]
            screen_x = -start_pos + (x * p)
            screen_y = start_pos - (y * p)
            pen.goto(screen_x, screen_y)
            pen.stamp()

def main():
    # Initial params for drawing
    turtle.resizemode("auto")
    turtle.delay(0)
    wn = turtle.Screen()
    wn.bgcolor("black")
    wn.title("Maze")
    wn.setup(800, 800)
    regex = re.compile(r'\d+')

    # maze_name = wn.textinput("", "Arquivo de labirinto:")
    maze_num = wn.numinput("", "Numero de labirinto:")
    alg_name = wn.textinput("", "Algoritmo: \nbusca_largura, busca_profundidade,\nbusca_best_first ou busca_Aestrela")
    # path_name = wn.textinput("", "Arquivo de Solução:")
    path_name = alg_name + "/caminho" + str(int(maze_num))
    dimensions, maze = read_maze(maze_num)

    # Retrieve the path from the file
    with open("caminhos/" + path_name + ".txt") as f:
        path = f.readline()
    path = literal_eval(path)

    square_size = 650/len(maze)
    pen = Pen(square_size)
    penpath = PenPath(square_size)
    setup_maze(maze, pen)
    print_path(path, penpath, maze)

    wn.tracer(0)
    turtle.mainloop()

if __name__ == '__main__':
    main()