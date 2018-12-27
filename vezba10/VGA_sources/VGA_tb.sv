module VGA_tb#()
   ();

   logic         clk = 0;
   logic         reset = 1;
   
   logic  hsync;
   logic  vsync;
   //bram interface
   logic [31:0] bram_addr;
   logic        bram_en;

   VGA #(.CLK_DIV_VAL(4))
   VGA_ip(.clk(clk),
          .reset(reset),
          .hsync(hsync),
          .vsync(vsync),
          .bram_addr(bram_addr),
          .bram_en(bram_en));

   initial begin
      reset = 0;
      #100ns reset = 1;
      
   end
   always
        #50ns clk <= ~clk;
endmodule
