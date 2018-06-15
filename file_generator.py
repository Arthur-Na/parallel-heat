import argparse

class FileGenerator():

    def __init__(self, x, y, z, wall_value, floor_value, ceiling_value, base_value=0):
        self.x = x
        self.y = y
        self.z = z
        self.wall_value = wall_value
        self.base_value = base_value
        self.floor_value = floor_value
        self.ceiling = ceiling_value

    def __call__(self, file, *args, **kwargs):
        with open(file, "w") as fp:
            self.generate_intro(fp)
            self.generate_walls(fp)
            self.generate_top_bottom(fp)

    def generate_intro(self, fp):
        fp.write("%i %i %i %d\n" % (self.x, self.y, self.z, self.base_value))

    def generate_walls(self, fp):
        for x in range(self.x):
            for z in range(self.z):
                fp.write("%i %i %i %d\n" % (x, 0, z, self.wall_value))
                fp.write("%i %i %i %d\n" % (x, self.y - 1, z, self.wall_value))

            for y in range(self.y):
                fp.write("%i %i %i %d\n" % (x, y, 0, self.wall_value))
                fp.write("%i %i %i %d\n" % (x, y, self.z - 1, self.wall_value))

    def generate_top_bottom(self, fp):
        for z in range(self.z):
            for y in range(self.y):
                fp.write("%i %i %i %d\n" % (0, y, z, self.floor_value))
                fp.write("%i %i %i %d\n" % (self.x - 1, y, z, self.ceiling))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Generate input files for heat diffusion')
    parser.add_argument('-x', type=int)
    parser.add_argument('-y', type=int)
    parser.add_argument('-z', type=int)
    parser.add_argument('-wall', type=float)
    parser.add_argument('-floor', type=float)
    parser.add_argument('-ceiling', type=float)

    args = parser.parse_args()

    fg = FileGenerator(args.x, args.y, args.z, args.wall, args.floor, args.ceiling)
    fg("test_file")
