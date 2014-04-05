library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity myrtc is
    port (clk, reset, slave_write: in std_logic;
          slave_address: in std_logic_vector(3 downto 0);
          slave_writedata: in std_logic_vector(7 downto 0);
          user_dataout_0: out std_logic_vector(7 downto 0));
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

    user_dataout_0 <= std_logic_vector(cnt(31 downto 28));
    
--    process (reset) begin
--        if reset = '1' then
--            dinges <= (others => '0');
--        end if;
--    end process;
end behavior;


