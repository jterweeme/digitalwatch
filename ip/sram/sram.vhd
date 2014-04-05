library ieee;
use ieee.std_logic_1164.all;

entity sram is
    port (clk, reset_n, s_chipselect_n, s_byteenable_n, s_write_n, s_read_n: in std_logic;
          s_address: in std_logic_vector(16 downto 0);
          s_writedata: in std_logic_vector(7 downto 0);
          s_readdata: out std_logic_vector(7 downto 0);
          sram_addr: out std_logic_vector(16 downto 0);
          sram_ce_n, sram_oe_n, sram_lb_n, sram_ub_n, sram_we_n: out std_logic;
          sram_dq: inout std_logic_vector(7 downto 0));
end sram;

architecture behavior of sram is
begin
    s_readdata <= sram_dq;
    sram_addr <= s_address;
    sram_we_n <= s_write_n;
    sram_oe_n <= s_read_n;
    sram_ce_n <= s_chipselect_n;
    sram_lb_n <= s_byteenable_n;
end behavior;

