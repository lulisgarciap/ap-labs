import pygame
import os

f = open("initFile.txt", "r")
init_values = f.read()

redCarImage = None
greenCarImage = None
blueCarImage = None

def initRed():
    global redCarImage
    redCarImage = pygame.image.load("res/red_car.png")
    redCarImage = pygame.transform.scale(redCarImage, (30,45))
    redCarImage = redCarImage.convert_alpha()

def initGreen():
    global greenCarImage
    greenCarImage = pygame.image.load("res/green_car.png")
    greenCarImage = pygame.transform.scale(greenCarImage, (30,45))
    greenCarImage = greenCarImage.convert_alpha()

def initBlue():
    global blueCarImage
    blueCarImage = pygame.image.load("res/blue_car.png")
    blueCarImage = pygame.transform.scale(blueCarImage, (30,45))
    blueCarImage = blueCarImage.convert_alpha()

pygame.init() # PYGAME Initilization
screen = pygame.display.set_mode((900,700))

####### BEGIN CARS DEFINITION ######
initBlue()
if int(init_values[0]) == 2:
    initGreen()
if int(init_values[0]) == 3:
    initGreen()
    initRed()
####### END CARS DEFINITION ########

backgroundImage = pygame.image.load("res/background.png")
backgroundImage = pygame.transform.scale(backgroundImage, (900,700))
screen.blit(backgroundImage, (0,0))

frame = pygame.time.Clock()

redX = 60
redY = 325
greenX = 100
greenY = 325
blueX = 140
blueY = 325

running = True

while running == True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    if os.path.exists("status.txt"):
        f = open("status.txt", "r")
        if f.mode == "r":
            contents = f.read().split()  ## FILE FORMAT: Car X Y Rotation
            if contents[0] == "Blue":   # Check if Blue car has written to the status file
                print("El Azul")
                carX = int(contents[1], 10)
                carY = int(contents[2], 10)
                screen.blit(backgroundImage, (0,0))
                screen.blit(blueCarImage, (carX,carY))
                print(carX, carY)
                print contents[3]
                rotation = int(contents[3], 10)
                ## TODO: Perform Rotation
                # blueCarImage = pygame.transform.rotate(blueCarImage,rotation)
                # Delete the FILE
                os.remove("status.txt")
                continue

            if contents[0] == "Red":    # Check if Red car has written to the status file
                print("El Rojo")
            if contents[0] == "Green":   # Check if Green car has written to the status file
                print("El Verde")

    pygame.display.flip()
    frame.tick(30)