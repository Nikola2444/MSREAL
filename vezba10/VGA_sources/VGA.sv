module VGA#
  (
   parameter integer CLK_DIV_VAL = 4   
   )
   (
    input logic         clk,
    input logic         reset,

    output logic        hsync,
    output logic        vsync,
    //bram interface
    output logic [31:0] bram_addr,
    output logic        bram_en
    );
   

   
   logic                         clk_en, clk_en_next;
   //constants
   localparam logic [9:0]        h_end = 800;
   localparam logic [9:0]        h_left_boundary = 48;
   localparam logic [9:0]        h_right_boundary = 16;
   localparam logic [9:0]        h_writeback = 96;
   localparam logic [9:0]        h_image = 640;

   localparam logic [9:0]        v_end = 525;
   localparam logic [9:0]        v_bottom_boundary = 10;
   localparam logic [9:0]        v_upper_boundary = 33;
   localparam logic [9:0]        v_writeback = 2;
   localparam logic [9:0]        v_image = 480;

   localparam logic [9:0]        v_resolution = 144;
   localparam logic [9:0]        h_resolution = 256;
   // registers
   logic [9:0]                   h_counter_reg, h_counter_next;
   logic [9:0]                   v_counter_reg, v_counter_next;
   
   logic                         hsync_reg, hsync_next;
   logic                         vsync_reg, vsync_next;
   logic [$clog2(CLK_DIV_VAL):0] clock_divider_reg, clock_divider_next;
   
   always_ff @(posedge clk) begin
      if (!reset) begin
         clock_divider_reg <= 0;
         h_counter_reg <= 0;
         v_counter_reg <= 0;         
         hsync_reg <= 0;
         vsync_reg <= 0;
         clk_en <= 0;
      end
      else begin
         clock_divider_reg <= clock_divider_next;
         h_counter_reg <= h_counter_next;
         v_counter_reg <= v_counter_next;
         hsync_reg <= hsync_next;
         vsync_reg <= vsync_next;
         clk_en <= clk_en_next;
      end
   end
   
   assign   clk_en_next = (clock_divider_reg == 4) ? 1 : 0;
   
   always_comb begin
      clock_divider_next = clock_divider_reg;      
      if(clock_divider_reg == 4) begin
         clock_divider_next = 0;
      end
      else begin
         clock_divider_next = clock_divider_reg + 1;                 
      end            
   end
   
   
   always_comb begin
      h_counter_next = h_counter_reg;      
      if (clk_en == 1) begin
         if (h_counter_reg < (h_end - 1))
           h_counter_next = h_counter_reg + 1;
         else
           h_counter_next = 0;
      end
   end
   
   always_comb begin
      v_counter_next = v_counter_reg;      
      if (clk_en == 1) begin
         if(h_counter_reg == (h_end - 1)) begin
            if (v_counter_reg < (v_end - 1))
              v_counter_next = v_counter_reg + 1;        
            else
              v_counter_next = 0;            
         end
      end
   end

   always_comb begin
      hsync_next = hsync_reg;
      // 655 <= h_counter <= 751
         if (h_counter_reg >= (h_image + h_right_boundary) && 
             h_counter_reg <= (h_image + h_right_boundary + h_writeback - 1))
           hsync_next = 0;
         else
           hsync_next = 1;
   end
   always_comb begin
      vsync_next = vsync_reg;
      // 490 <= h_counter <= 491
         if (v_counter_reg >= (v_image + v_bottom_boundary) && 
             v_counter_reg <= (v_image + v_bottom_boundary + v_writeback - 1))
           vsync_next = 0;         
         else
           vsync_next = 1;
   end

   always_comb begin
      bram_en = 1;
         if (h_counter_reg < h_resolution && v_counter_reg < v_resolution)
           bram_addr = (h_counter_reg + v_counter_reg * h_resolution) * 4;    
         else
           bram_addr = (h_resolution * v_resolution + 1) * 4;         
   end
   assign vsync = vsync_reg;
   assign hsync = hsync_reg;


   
endmodule
