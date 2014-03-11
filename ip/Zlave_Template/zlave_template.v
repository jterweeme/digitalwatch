module zlave_template (
    clk, reset, slave_address, slave_read,
    slave_write, slave_readdata, slave_writedata,
    slave_byteenable, user_dataout_0, user_datain_0,
    user_chipselect, user_byteenable, user_write, user_read);

	parameter MODE_0 = 2;

	input clk, reset, slave_read, slave_write, slave_byteenable;
	input [3:0] slave_address;
	output reg [7:0] slave_readdata;
	input [7:0] slave_writedata, user_datain_0;
	output wire [7:0] user_dataout_0;
	output wire [15:0] user_chipselect;
	output wire user_byteenable, user_write, user_read;
	wire internal_byteenable;
	reg internal_byteenable_d1, slave_read_d1, slave_read_d2, slave_write_d1;
	wire [15:0] address_decode;
	reg [15:0] address_decode_d1;
	wire [3:0] address_bank_decode;
	reg [3:0] address_bank_decode_d1;
    reg [7:0] user_datain_0_d1,mux_first_stage_a,mux_first_stage_b,mux_first_stage_c,mux_first_stage_d;
	
    assign internal_byteenable = 1'b1;
	assign address_decode[0] = (slave_address == 4'b0000) & (slave_write | slave_read);
	assign address_decode[1] = (slave_address == 4'b0001) & (slave_write | slave_read);
	assign address_decode[2] = (slave_address == 4'b0010) & (slave_write | slave_read);
	assign address_decode[3] = (slave_address == 4'b0011) & (slave_write | slave_read);
	assign address_bank_decode[0] = (address_decode_d1[3:0] != 0)? 1'b1 : 1'b0;
	assign address_bank_decode[1] = (address_decode_d1[7:4] != 0)? 1'b1 : 1'b0;
	assign address_bank_decode[2] = (address_decode_d1[11:8] != 0)? 1'b1 : 1'b0;
	assign address_bank_decode[3] = (address_decode_d1[15:12] != 0)? 1'b1 : 1'b0;
		
	always @ (posedge clk or posedge reset)
	begin
		if (reset == 1)
		begin
			slave_read_d1 <= 0;
			slave_read_d2 <= 0;
			slave_write_d1 <= 0;
			address_decode_d1 <= 0;
			address_bank_decode_d1 <= 0;
			internal_byteenable_d1 <= 0;
			user_datain_0_d1 <= 0;
		end
		else
		begin
			slave_read_d1 <= slave_read;
			slave_read_d2 <= slave_read_d1;
			slave_write_d1 <= slave_write;
			internal_byteenable_d1 <= internal_byteenable;
			if((slave_read == 1) | (slave_write == 1))
				address_decode_d1 <= address_decode;
			if(slave_read_d1 == 1)
				address_bank_decode_d1 <= address_bank_decode;
			if((address_decode[0] == 1) & (slave_read == 1))
				user_datain_0_d1 <= user_datain_0;
		end
	end
	
	register_with_bytelanes register_0(clk,reset,slave_writedata,
            slave_write&address_decode[0],internal_byteenable,user_dataout_0);

/*
    always @ (posedge clk or posedge reset)
	begin
		if(reset == 1)
            user_dataout_0[7:0] <= 0;
		else if((internal_byteenable == 1) & ((slave_write & address_decode[0]) == 1))
            user_dataout_0[7:0] <= slave_writedata[7:0];
	end
*/

	always @ (posedge clk or posedge reset)
	begin
		if (reset == 1)
			slave_readdata <= 0;
		else
		begin
			if (slave_read_d1 == 1)
			begin
				case (address_decode_d1[3:0])
					4'b0001: mux_first_stage_a <= (MODE_0 == 3)? user_dataout_0 : user_datain_0_d1;
				endcase
			end
			
			if (slave_read_d2 == 1)
			begin
				case (address_bank_decode_d1[3:0])
					4'b0001: slave_readdata <= mux_first_stage_a;
					4'b0010: slave_readdata <= mux_first_stage_b;
					4'b0100: slave_readdata <= mux_first_stage_c;
					4'b1000: slave_readdata <= mux_first_stage_d;
				endcase
			end
		end
	end
	
    assign user_write = slave_write_d1; 
    assign user_read = slave_read;
    assign user_chipselect = (slave_write_d1 == 1)? address_decode_d1 : address_decode;
    assign user_byteenable = (slave_write_d1 == 1)? internal_byteenable_d1 : internal_byteenable;  
endmodule

module register_with_bytelanes(clk, reset, data_in, write, byte_enables, data_out);
	input clk, reset, write, byte_enables;
	input [7:0] data_in;
	output reg [7:0] data_out;

	always @ (posedge clk or posedge reset)
	begin
		if(reset == 1)
            data_out[7:0] <= 0;
		else if((byte_enables == 1) & (write == 1))
            data_out[7:0] <= data_in[7:0];
	end
endmodule


