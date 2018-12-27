library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
entity VGA_ip is
  port(clk: in std_logic;
       --BRAM INTERFACE       
       bram_addr: out std_logic_vector(31 downto 0);
       bram_en: out std_logic_vector(31 downto 0);
       bram_we: out std_logic_vector(31 downto 0);
       --VGA interface
       hsync: out std_logic;
       vsync: out std_logic;
       video_on: out std_logic);

end entity VGA_ip;            


architecture behavioral of VGA_ip is
  signal mod_5:std_logic_vector(2 downto 0);
  signal hsync_counter, vsync_counter: std_logic_vector(9 downto 0):=(others =>'0');
  signal clk_divider: std_logic;
  constant hsync_cnt_end: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(799, 10));
  constant vsync_cnt_end: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(524, 10));
  constant h_left_boundary: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(48, 10));
  constant h_right_boundary: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(16, 10));
  constant h_writeback: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(96, 10));
  constant h_image: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(640, 10));
  constant v_bottom_boundary: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(10, 10));
  constant v_upper_boundary: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(33, 10));
  constant v_writeback: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(2, 10));
  constant v_image: std_logic_vector(9 downto 0):=std_logic_vector(to_unsigned(480, 10));
  
  
begin
  clock_divider: process(clk)
  begin
    if rising_edge(clk)then
      if (mod_5 < "101") then
        mod_5 <= std_logic_vector(unsigned(mod_5) + 1);
        clk_divider <= '0';
      else
        mod_5 <= (others =>'0');
        clk_divider <= '1';
      end if; 
    end if;
  end process;
  
  hsync_cnt_proc: process(clk_divider)
  begin
    if rising_edge(clk_divider)then
      if (hsync_counter = hsync_cnt_end) then
        hsync_counter <= (others => '0');
      else
        hsync_counter <= std_logic_vector(unsigned(hsync_counter) + 1);
      end if; 
    end if;
  end process;

  vsync_cnt_proc: process(clk_divider)
  begin
    if rising_edge(clk_divider)then
      if (hsync_counter = hsync_cnt_end)then
        if (vsync_counter = vsync_cnt_end) then
          vsync_counter <= (others => '0');
        else
          vsync_counter <= std_logic_vector(unsigned(vsync_counter) + 1);
        end if;
      end if;
    end if;
  end process;

  hsync_gen_proc: process(clk_divider)
  begin
    if rising_edge(clk_divider)then
      -- 640 <= vsync_counter < 751
      if (hsync_counter >= h_image and hsync_counter <= std_logic_vector(unsigned(h_image) + unsigned(h_right_boundary) + unsigned(h_writeback))) then        
        hsync <=  '0';
      else
        hsync <= '1';
      end if; 
    end if;
  end process;
  vsync_gen_proc: process(clk_divider)
  begin
    if rising_edge(clk_divider)then
      -- 490 <= vsync_counter < 492
      if (vsync_counter >= std_logic_vector(unsigned(v_image) + unsigned(v_bottom_boundary)) and
          vsync_counter < std_logic_vector(unsigned(v_image) + unsigned(v_bottom_boundary) + unsigned(v_writeback))) then
        vsync <=  '0';
      else
        vsync <= '1';
      end if; 
    end if;
  end process;

  bram_add_gen_proc: process(clk_divider)
  begin
    if rising_edge(clk_divider)then
      if ((unsigned(vsync_counter)*unsigned(hsync_counter)) >= 256*144)  then
        bram_addr <= std_logic_vector(to_unsigned(256*144, 31));
      else
        bram_addr <= std_logic_vector(unsigned(vsync_counter)*unsigned(hsync_counter));
      end if; 
    end if;
  end process;
end behavioral;





  
