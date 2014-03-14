library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity leds2 is
    port (clk, reset, slave_write: in std_logic;
          slave_address: in std_logic_vector(3 downto 0);
          slave_writedata: in std_logic_vector(7 downto 0);
          user_dataout_0: out std_logic_vector(7 downto 0));
end leds2;

architecture behavior of leds2 is
begin
    ledjes: process(clk, reset) begin
        if reset = '1' then
            user_dataout_0 <= (others => '0');
        elsif slave_write = '1' then
            user_dataout_0 <= slave_writedata;
        end if;
    end process;
end behavior;

