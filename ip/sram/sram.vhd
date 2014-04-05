-- 2014 Jasper ter Weeme

library ieee;
use ieee.std_logic_1164.all;

entity sram is
    port (clk, reset_n, s_chipselect_n, s_byteenable_n, s_write_n, s_read_n: in std_logic;
          s_address: in std_logic_vector(16 downto 0);
          s_writedata: in std_logic_vector(7 downto 0);
          s_readdata: out std_logic_vector(7 downto 0);
          SRAM_ADDR: out std_logic_vector(16 downto 0);
          SRAM_CE_n, SRAM_OE_n, SRAM_LB_n, SRAM_UB_n, SRAM_WE_n: out std_logic;
          SRAM_DQ: inout std_logic_vector(7 downto 0));
end sram;

architecture behavior of sram is
begin
    SRAM_DQ <= (others => 'Z') when s_write_n = '1' else s_writedata;
    s_readdata <= SRAM_DQ;
    SRAM_ADDR <= s_address;
    SRAM_WE_n <= s_write_n;
    SRAM_OE_n <= s_read_n;
    SRAM_CE_n <= s_chipselect_n;
    SRAM_LB_n <= s_byteenable_n;
end behavior;

