#! /usr/bin/python3.6

import argparse


class FileGenerator:

    def __init__(self, x, y, z, wall_value, floor_value, ceiling_value, base_value=0):
        self.x = x
        self.y = y
        self.z = z
        self.wall_value = wall_value
        self.base_value = base_value
        self.floor_value = floor_value
        self.ceiling = ceiling_value

    def __call__(self, f, *args, **kwargs):
        with open(f, "w") as fp:
            self.generate_intro(fp)
            self.generate_walls(fp)
            self.generate_floor_ceiling(fp)

    def generate_intro(self, fp):
        fp.write("%i %i %i %f\n" % (self.x, self.y, self.z, self.base_value))

    def generate_walls(self, fp):
        for x in range(self.x):
            for z in range(self.z):
                fp.write("%i %i %i %f\n" % (x, 0, z, self.wall_value))
                fp.write("%i %i %i %f\n" % (x, self.y - 1, z, self.wall_value))

            for y in range(self.y):
                fp.write("%i %i %i %f\n" % (x, y, 0, self.wall_value))
                fp.write("%i %i %i %f\n" % (x, y, self.z - 1, self.wall_value))

    def generate_floor_ceiling(self, fp):
        for z in range(self.z):
            for y in range(self.y):
                fp.write("%i %i %i %f\n" % (0, y, z, self.floor_value))
                fp.write("%i %i %i %f\n" % (self.x - 1, y, z, self.ceiling))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Generate input files for heat diffusion')
    parser.add_argument('x', type=int, help='x size')
    parser.add_argument('y', type=int, help='y size')
    parser.add_argument('z', type=int, help='z size')
    parser.add_argument('wall', type=float, help='wall heat value')
    parser.add_argument('filename', type=str, help='filename')
    parser.add_argument('-f', '--floor', type=float, help='floor heat value, wall heat value default')
    parser.add_argument('-c', '--ceiling', type=float, help='ceiling heat value, wall heat value default')
    parser.add_argument('-b', '--base', type=float, help='base heat value, 0 default')

    args = parser.parse_args()

    floor = args.floor if args.floor else args.wall
    ceiling = args.ceiling if args.ceiling else args.wall
    base = args.base if args.base else 0

    fg = FileGenerator(args.x, args.y, args.z, args.wall, floor, ceiling, base_value=base)
    fg(args.filename)
