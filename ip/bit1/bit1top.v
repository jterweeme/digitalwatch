module bit1top(
    input [2:0] avs_s1_address,
    input chipselect,
    input csi_clk,
    input csi_reset,
    input avs_s1_write,
    input [31:0] avs_s1_writedata,
    inout wire coe_bit,
    output reg [31:0] avs_s1_readdata
);
    wire clk_en;
    reg data_dir;
    wire data_in;
    reg data_out;
    wire read_mux_out;
    wire wr_strobe;
    assign clk_en = 1;

    assign read_mux_out = ({1 {(avs_s1_address == 0)}} &
        data_in) | ({1 {(avs_s1_address == 1)}} & data_dir);

    assign wr_strobe = chipselect && avs_s1_write;
    assign coe_bit = data_dir ? data_out : 1'bZ;
    assign data_in = coe_bit;

    always @(posedge csi_clk or posedge csi_reset) begin
        if (csi_reset)
            avs_s1_readdata <= 0;
        else if (clk_en)
            avs_s1_readdata <= {32'b0 | read_mux_out};
    end

    always @(posedge csi_clk or posedge csi_reset) begin
        if (csi_reset)
            data_out <= 0;
        else if (clk_en)
            if (wr_strobe)
                data_out <= (avs_s1_address == 5) ? data_out & ~avs_s1_writedata :
                    (avs_s1_address == 4) ? data_out | avs_s1_writedata :
                    (avs_s1_address == 0)? avs_s1_writedata : data_out;
    end

    always @(posedge csi_clk or posedge csi_reset) begin
        if (csi_reset)
            data_dir <= 0;
        else if (chipselect && ~avs_s1_write && (avs_s1_address == 1))
            data_dir <= avs_s1_writedata;
    end



endmodule





