set base 0x24000000

proc rdreg { name } {
    set re [format "$name%s" {[^:]*: }];
    set r [reg $name force];
    regsub -- $re $r "" r2;
    return $r2
}

set Reset_Handler [format "0x%08X" [expr ([mrw [expr $base + 0x04]])]]
set ControlBlock  [format "0x%08X" [expr ([mrw [expr $base + 0x1C]])]]

set MemBuffer     [format "0x%08X" [expr ([mrw [expr $ControlBlock + 0x00]])]]
set MemBufferSize [format "%d" [expr ([mrw [expr $ControlBlock + 0x04]])]]
set ErasePage     [format "0x%08X" [expr ([mrw [expr $ControlBlock + 0x08]])]]
set FlashCopy     [format "0x%08X" [expr ([mrw [expr $ControlBlock + 0x0C]])]]
set FlashVerify   [format "0x%08X" [expr ([mrw [expr $ControlBlock + 0x10]])]]

puts "Reset_Handler $Reset_Handler"
puts "ControlBlock  $ControlBlock"
puts "  MemBuffer     $MemBuffer"
puts "  MemBufferSize $MemBufferSize bytes"
puts "  ErasePage     $ErasePage"
puts "  FlashCopy     $FlashCopy"
puts "  FlashVerify   $FlashVerify"

proc doErasePage { a } {
    global ErasePage
    reg r0 $a
    resume $ErasePage
    wait_halt 30000
    set result [rdreg r0]
    puts "return $result"
    return $result
}

proc doFlashCopy { a z } {
    global FlashCopy
    reg r0 $a
    reg r1 $z
    resume $FlashCopy
    wait_halt 30000
    set result [rdreg r0]
    puts "return $result"
    return $result
}

proc doFlashVerify { a z } {
    global FlashVerify
    reg r0 $a
    reg r1 $z
    resume $FlashVerify
    wait_halt 3000
    set result [rdreg r0]
    puts "return $result"
    return $result
}

proc doInitTarget {} {
    global Reset_Handler
    resume $Reset_Handler
    wait_halt 3000
    set result [rdreg r0]
    puts "return $result"
    return $result
}

proc load_bin {fname foffset address length } {
    load_image $fname [expr $address - $foffset] bin $address $length
}
