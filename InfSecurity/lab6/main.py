import struct, os

N = 256

class Node():
    def __init__(self, value, freq, parent = None, left = None, right = None):
        self.value = value
        self.freq = freq
        self.parent = parent
        self.left = left
        self.right = right
    
    @staticmethod
    def getFrequency(node):
        return node.freq


class Huffman():
    def __init__(self, filename):
        self.table = self.getFreqTable(filename)
        self.codes_table = self.getCodesTable()

    def getFreqTable(self, filename):
        table = [0] * N
        with open(filename, "rb") as f:
            data = f.read()
        for item in data:
            table[item] += 1
        return table                    

    def getCodesTable(self):
        nodes = []
        leafs = []
        codes = []

        for i in range(N):
            if self.table[i] > 0:
                nodes.append(Node(i, self.table[i]))

        if len(nodes) == 1:
            leafs.append(nodes[0])
            codes.append((nodes[0].value, "0"))
            return codes

        while len(nodes) > 1:
            nodes.sort(key = Node.getFrequency)
            left, right = nodes.pop(0), nodes.pop(0)
            parent = Node(None, left.freq + right.freq, None, left, right) 
            left.parent = parent
            right.parent = parent

            if (left.value != None):
                leafs.append(left)
            if (right.value != None):
                leafs.append(right)
            nodes.append(parent)
        
        for leaf in leafs:
            code = ""
            node = leaf

            while True:
                parent = node.parent
                if parent == None:
                    break
                if node == parent.left:
                    code += "0"
                if node == parent.right:
                    code += "1"
                node = parent
            codes.append((leaf.value, code[::-1]))
        return codes

    def bitsToByte(self, bit_str):
        return struct.pack('B', int(bit_str, 2))

    def getCode(self, byte):
        for i in range(len(self.codes_table)):
            if self.codes_table[i][0] == byte:
                return self.codes_table[i][1]

    def getByte(self, code):
        for i in range(len(self.codes_table)):
            if self.codes_table[i][1] == code:
                return self.codes_table[i][0]
        return None

    def compress(self, filename):      
        zeros = 0
        res_fname = "compressed_" + filename
        with open(filename, "rb")  as f:
            data = f.read()

        res_f = open(res_fname, "wb")
        code_str = ""

        for item in data:
            code = self.getCode(item)
            code_str += code
            while len(code_str) >= 8:
                bits_str = code_str[:8]
                code_str = code_str[8:]
                byte = self.bitsToByte(bits_str)
                res_f.write(byte)       

        if len(code_str) > 0:
            zeros = 8 - len(code_str)
            code_str += '0' * zeros
            byte = self.bitsToByte(code_str)
            res_f.write(byte)

        res_f.close()
        return zeros

    def decompress(self, compr_fname, zeros):
        with open(compr_fname, "rb") as f:
            data = f.read()
        
        res_fname = "de" + compr_fname
        res_f = open(res_fname, "wb")
        code_str = ""

        for byte in data:
            code = bin(byte)[2:].zfill(8)
            code_str += code

        if zeros:
            code_str = code_str[:-zeros]

        code = ""
        while len(code_str):
            code += code_str[0]
            code_str = code_str[1:]
            byte = self.getByte(code)

            if byte != None:
                res_f.write(struct.pack('B', byte))
                code = ""

        res_f.close()

def main():
    filename = input('Введите имя файла для сжатия: ')
    print("Исходный файл: {} байт".format(os.path.getsize(filename)))
    huf = Huffman(filename)

    compr_filename = "compressed_" + filename
    zeros = huf.compress(filename)
    print("Сжатый файл: {} байт".format(os.path.getsize(compr_filename)))

    decompr_filename = "decompressed_" + filename
    huf.decompress(compr_filename, zeros)
    print("Восстановленный файл: {} байт".format(os.path.getsize(decompr_filename)))

if __name__ == '__main__':
    main()