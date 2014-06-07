-- 2014 Jasper ter Weeme

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity leds2 is
    port (csi_clk, csi_reset, slave_write: in std_logic;
          slave_address: in std_logic_vector(3 downto 0);
          slave_writedata: in std_logic_vector(7 downto 0);
          user_dataout_0: out std_logic_vector(7 downto 0));
end leds2;

architecture behavior of leds2 is
begin
    ledjes: process(csi_clk, csi_reset) begin
        if csi_reset = '1' then
            user_dataout_0 <= (others => '0');
        elsif slave_write = '1' and rising_edge(csi_clk) then
            user_dataout_0 <= slave_writedata;
        end if;
    end process;
end behavior;

