source [find interface/cmsis-dap.cfg]
transport select swd

source [find target/stm32h7x.cfg]

init

reset halt
load_image $firmware
verify_image $firmware
arm semihosting enable
arm semihosting_resexit enable
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
    wait_halt 3000
    set result [rdreg r0]
    puts "return $result"
    return $result
}

proc doFlashCopy { a z } {
    global FlashCopy
    reg r0 $a
    reg r1 $z
    resume $FlashCopy
    wait_halt 3000
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

reset halt

doInitTarget

puts "Writing file $binary_file to memory"

set size [file size $binary_file]
echo "$binary_file size: $size"

set addr 0x00000000
set readed 0
while { $readed < $size } {
    set percent [expr ($readed * 100.0) / $size]
    puts [format "Programming at offset %d of %d (%3.02f%%)" $readed $size $percent]
    load_bin $binary_file $readed $MemBuffer $MemBufferSize
    if { [doErasePage $addr] != 0x00000000 } {
        puts "Error erasing at addr $addr"
        break
    }
    if { [doFlashCopy $addr $MemBufferSize] != 0x00000000 } {
        puts "Error erasing at addr $addr"
        break
    }
    if { [doFlashVerify $addr $MemBufferSize] != 0x00000000 } {
        puts "Error erasing at addr $addr"
        break
    }
    set addr [expr $addr + $MemBufferSize]
    set readed [expr $readed + $MemBufferSize]
}
puts "End of program"

exit
