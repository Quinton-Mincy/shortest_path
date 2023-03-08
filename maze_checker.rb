# Author: Gaetan Juvin
# Date: 1 CGE (Common Gaetan Era)
#
# Description: Confirms if the maze solving algorithm is succsseful.
#
# Contributors:
# - Quinton Mincy, February 15, 2023: Corrected a bug that caused checker to only work if the exit was directly across from the entrance (no turns).


class MouseResolver
    attr_accessor :data
    attr_accessor :header_line
    attr_accessor :last_line
    attr_accessor :map_height, :map_width
    attr_accessor :chars

    def initialize
        self.data = []
        self.chars = ""
        self.map_height = 0
        self.map_width = 0
    end

    def load(data)
        index = 0

        data.each do |line|
            if index == 0
                self.setHeaderLine(line)
            elsif index <= self.map_height
                self.data << line
            else
                self.setLastLine(line)
            end
            index += 1
        end
    end

    def setHeaderLine(header_line)
        @header_line    = header_line
        self.map_height = header_line.to_i
        x_pos = header_line.index('x')
        raise "Invalid header, expected HEIGHTxWIDTH* o12 got #{header_line}" if x_pos == nil
        self.map_width  = header_line[x_pos+1..-1].to_i
        self.chars = header_line[(self.map_height.to_s.size + self.map_width.to_s.size) + 1..-1].strip
    end

    def setLastLine(last_line)
        @last_line = last_line
    end

    def print
        p self.map_height
        p self.map_width
        p self.data
        p self.chars
    end

    def print_map
        self.data.each do |line|
            puts line
        end
    end

    def _find_entrance
        y = 0
        x = 0
        while y < self.data.size
            while x < self.data[y].size
                if self.data[y][x] == '1'
                    return [y, x]
                end
                x += 1
            end
            y += 1
        end
        [-1, -1]
    end

    def _where_to(y, x)
        [[y - 1, x], [y, x - 2], [y, x + 2], [y + 1, x]].each do |future_y, future_x|
            next if future_y < 0 or future_x < 0
            next if future_y > self.map_height
            next if future_x > self.map_width

            if [self.chars[2], self.chars[4]].include?(self.data[future_y][future_x])
                return [future_y, future_x]
            end
        end
        [-1, -1]
    end

    def resolve
        y, x = _find_entrance
        step = 0
        
        loop do
            self.print_map
            if self.data[y][x] == self.chars[4]
                return step
            end
            if y == -1 or x == -1
                break
            end
            self.data[y][x] = '+'
            y, x = _where_to(y, x)
            step += 1
        end
        -1
    end


    def self.resolve(str_map)
      mr = MouseResolver.new
      mr.load(str_map.split("
"))
      nbr_step = mr.resolve
      raise 'Exit not reached, invalid solution' if nbr_step == -1
      nbr_step
    end
end

def get_argf
    data = []
    ARGF.each do |line|
        data << line.strip
    end
    data
end

data = get_argf

p MouseResolver.resolve(data.join("\n"))
