mem = {}

function mem_read(addr)
   return mem[addr]
end

function mem_write(addr, val)
   mem[addr] = val
end

mem.read = mem_read
mem.write = mem_write
