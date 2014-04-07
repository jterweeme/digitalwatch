-- 2014 Jasper ter Weeme

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity myrtc is
    port (clk, reset: in std_logic;
          slave_address: in std_logic_vector(3 downto 0);
          slave_readdata: out std_logic_vector(7 downto 0));
end myrtc;

architecture behavior of myrtc is
    signal cnt: unsigned(31 downto 0);
    signal dinges: unsigned(3 downto 0);
begin
    klokdeler: process (clk, reset) begin
        if reset = '1' then
            cnt <= (others => '0');
        elsif rising_edge(clk) then
            cnt <= cnt + 1;
        end if;
    end process;

    slave_readdata <= std_logic_vector(cnt(31 downto 24));
    
--    process (reset) begin
--        if reset = '1' then
--            dinges <= (others => '0');
--        end if;
--    end process;
end behavior;


