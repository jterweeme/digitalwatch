/*
Jasper ter Weeme
*/

module vga2(
    csi_clk50,
    csi_reset_n,
    coe_hsync,
    coe_vsync,
    coe_red,
    coe_green,
    coe_blue,
    coe_clk25
);
    parameter BITS_PER_COLOR = 1;
    input csi_clk50;
    input csi_reset_n;
    output coe_hsync;
    output coe_vsync;
    output [BITS_PER_COLOR - 1:0] coe_red;
    output [BITS_PER_COLOR - 1:0] coe_green;
    output [BITS_PER_COLOR - 1:0] coe_blue;
    output reg coe_clk25;
    wire r, g, b;
    
    generate
        if (BITS_PER_COLOR == 8) begin
            assign coe_red = {r, r, r, r, r, r, r, r};
            assign coe_green = {g, g, g, g, g, g, g, g};
            assign coe_blue = {b, b, b, b, b, b, b, b};
        end else begin
            assign coe_red = r;
            assign coe_green = g;
            assign coe_blue = b;
        end
    endgenerate

    always @(posedge csi_clk50)
        coe_clk25 <= ~coe_clk25;

    display vga_inst(
        .clk(coe_clk25),
        .hsync(coe_hsync),
        .vsync(coe_vsync),
        .red(r),
        .green(g),
        .blue(b));
endmodule

