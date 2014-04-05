-- 2014 Jasper ter Weeme

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity klokdeler is
    port (clk_in: in std_logic; clk_out: inout std_logic);
end klokdeler;

architecture behavior of klokdeler is
begin
    klok: process begin
        wait until rising_edge(clk_in);

        if clk_out = '0' then
            clk_out <= '1' after 2 ns;
        else
            clk_out <= '0' after 2 ns;
        end if;
    end process;
end behavior;


