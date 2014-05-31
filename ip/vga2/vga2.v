/*
Jasper ter Weeme
*/

module vga2(
    input csi_clk50,
    input csi_reset_n,
    output coe_hsync,
    output coe_vsync,
    output [7:0] coe_red,
    output [7:0] coe_green,
    output [7:0] coe_blue,
    output reg coe_clk25
);
    wire r, g, b;
    assign coe_red = {r, r, r, r, r, r, r, r};
    assign coe_green = {g, g, g, g, g, g, g, g};
    assign coe_blue = {b, b, b, b, b, b, b, b};

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

