----------------------------------------------------------------------------------
-- Company: UCSB
-- Engineer: Yuepei Hu
-- 
-- Create Date:    09:22:42 11/24/2011 
-- Design Name: 
-- Module Name:    dma - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity dma is
    Port ( dma_Clk : in  STD_LOGIC;
           slv_reg0 : in  STD_LOGIC_VECTOR (0 to 31);
           slv_reg1 : in  STD_LOGIC_VECTOR (0 to 31);
           data_in : in  STD_LOGIC_VECTOR (8 downto 0);
			  data_out : out  STD_LOGIC_VECTOR (8 downto 0);
           addr_out : out  STD_LOGIC_VECTOR (12 downto 0);
           wea : out  STD_LOGIC);
end dma;

architecture Behavioral of dma is
signal reg0: std_logic_vector (0 to 31);
signal reg1: std_logic_vector (0 to 31);
signal temp: STD_LOGIC_VECTOR (8 downto 0);
type state_type is (st0_null,st1_read, st2_gen, st3_write); 
signal state, next_state : state_type :=st0_null; 

begin
	
	process (slv_reg0)
	begin
		reg0<=slv_reg0;
	end process;
	
	process (slv_reg1,dma_Clk,state)
	begin
		if (dma_Clk'event and dma_Clk ='1')then
			if (state = st1_read) then
				reg1(31)<='0';
			else 
				reg1(31)<=slv_reg1(31);
			end if;
		end if;
	end process;
	

--Insert the following in the architecture after the begin keyword
   SYNC_PROC: process (dma_Clk)
   begin
      if (dma_Clk'event and dma_Clk = '1') then
         if (reg1(31)= '1') then
            state <= st1_read;
         else
            state <= next_state;
         -- assign other outputs to internal signals
         end if;        
      end if;
   end process;
 
   --MEALY State-Machine - Outputs based on state and inputs
   OUTPUT_DECODE: process (state)
   begin
      --insert statements to decode internal output signals
      --below is simple example
      if (state = st1_read) then
			temp <= data_in;
			reg1(30) <= '0';
      elsif (state = st2_gen) then
			reg1(30) <= '0';
			case (reg0(26 to 27)) is
				when "00" =>
					data_out <= temp(8 downto 3)&reg0(29 to 31);
				when "01" =>
					data_out <= temp(8 downto 6)&reg0(29 to 31)&temp(2 downto 0);
				when "10" =>
					data_out <= reg0(29 to 31)&temp(5 downto 0);
				when others => null;
			end case;
		elsif (state = st3_write) then
			reg1(30) <= '0';
			wea<='1';
		else
			reg1(30) <= '1';
			wea<='0';
			data_out <= "XXXXXXXXX";
      end if;
   end process;
 
   NEXT_STATE_DECODE: process (state)
   begin
      --declare default state for next_state to avoid latches
      next_state <= state;  --default is to stay in current state
      --insert statements to decode next_state
      --below is a simple example
      case (state) is
         when st1_read =>
            next_state <= st2_gen;
         when st2_gen =>
            next_state <= st3_write;
         when st3_write =>
            next_state <= st0_null;
         when others =>
            next_state <= st0_null;
      end case;      
   end process;

				
	
	addr_out <= reg0(11 to 23);
	
end Behavioral;

