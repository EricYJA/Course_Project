
/**
 * This class provides all code necessary to take a query box and produce
 * a query result. The getMapRaster method must return a Map containing all
 * seven of the required fields, otherwise the front end code will probably
 * not draw the output correctly.
 */
public class Rasterer {
    /** The max image depth level. */
    public static final int MAX_DEPTH = 7;
    public static final double ALL_ULLON = -122.2998046875; //origin left
    public static final double ALL_LRLON = -122.2119140625; // max right
    public static final double ALL_ULLAT = 37.892195547244356; // max up
    public static final double ALL_LRLAT = 37.82280243352756; // origin down


    /**
     * Takes a user query and finds the grid of images that best matches the query. These images
     * will be combined into one big image (rastered) by the front end. The grid of images must obey
     * the following properties, where image in the grid is referred to as a "tile".
     * <ul>
     *     <li>The tiles collected must cover the most longitudinal distance per pixel (LonDPP)
     *     possible, while still covering less than or equal to the amount of longitudinal distance
     *     per pixel in the query box for the user viewport size.</li>
     *     <li>Contains all tiles that intersect the query bounding box that fulfill the above
     *     condition.</li>
     *     <li>The tiles must be arranged in-order to reconstruct the full image.</li>
     * </ul>
     * @param params The RasterRequestParams containing coordinates of the query box and the browser
     *               viewport width and height.
     * @return A valid RasterResultParams containing the computed results.
     */
    public RasterResultParams getMapRaster(RasterRequestParams params) {

        /** Corner Cases
         */
        if (params.ullon >= params.lrlon || params.lrlat >= params.ullat) {
            return RasterResultParams.queryFailed();
        }

        if (params.lrlat >= ALL_ULLAT || params.ullat <= ALL_LRLAT
                || params.ullon >= ALL_LRLON || params.lrlon <= ALL_ULLON) {
            return RasterResultParams.queryFailed();
        }

        /**
         * this is for calculating the right depth
         * Note that the currentLonDpp may have error
         */

        int currentDepth = 0;
        double requestedLonDPP = (params.lrlon - params.ullon) / params.w;
        double currentLonDpp = (ALL_LRLON - ALL_ULLON) / 256; // calculate the current lonDPP

        while (requestedLonDPP < currentLonDpp && currentDepth < 7) {
            currentDepth++;
            currentLonDpp = ((ALL_LRLON / Math.pow(2, currentDepth))
                    - (ALL_ULLON / Math.pow(2, currentDepth))) / 256;
        }

        /**
         * this is for finding the upper left corner
         * and the downer right corner
         */

        double eachCOLlon =  ((ALL_LRLON / Math.pow(2, currentDepth))
                                    - (ALL_ULLON / Math.pow(2, currentDepth)));
        double eachROWlat = ((ALL_ULLAT / Math.pow(2, currentDepth))
                                - (ALL_LRLAT / Math.pow(2, currentDepth)));

        int currentuprow = (int) ((ALL_ULLAT - params.ullat) / eachROWlat);
        if (currentuprow < 0) {
            currentuprow = 0;
        }

        int currentleftcol = (int) ((params.ullon - ALL_ULLON) / eachCOLlon);
        if (currentleftcol < 0) {
            currentleftcol = 0;
        }

        int currentlowrow = (int) ((ALL_ULLAT - params.lrlat) / eachROWlat);
        if (currentlowrow > (Math.pow(2, currentDepth) - 1)) {
            currentlowrow = (int) (Math.pow(2, currentDepth) - 1);
        }

        int currentrightcol = (int) ((params.lrlon - ALL_ULLON) / eachCOLlon);
        if (currentrightcol > (Math.pow(2, currentDepth) - 1)) {
            currentrightcol = (int) (Math.pow(2, currentDepth) - 1);
        }

        /**
         * generate the pics we need to display
         */
        String[][] requiredPics = new String [currentlowrow - currentuprow + 1]
                                                [currentrightcol - currentleftcol + 1];
        int iCounter = 0;

        for (int i = currentuprow; i <= currentlowrow; i++) {
            int jCounter = 0;
            for (int j = currentleftcol; j <= currentrightcol; j++) {
                requiredPics[iCounter][jCounter] = "d" + currentDepth
                                            + "_x" + j + "_y" + i + ".png";
                jCounter += 1;
            }
            iCounter += 1;
        }

        return genResult(currentDepth, requiredPics, eachCOLlon, eachROWlat,
                currentleftcol, currentuprow, currentrightcol, currentlowrow).create();
    }


    private RasterResultParams.Builder genResult(int currentDepth, String[][] requiredPics,
                                                 double eachCOLlon, double eachROWlat,
                                                 double currentleftcol, double currentuprow,
                                                 double currentrightcol, double currentlowrow) {

        /**
         * generate the final coordinate of the raster and make sure that it fells in the range
         */

        double rasterullon = (eachCOLlon * (currentleftcol)) + ALL_ULLON;
        double rasterullat = ALL_ULLAT - (eachROWlat * (currentuprow));
        double rasterlrlon = (eachCOLlon * (currentrightcol + 1))  + ALL_ULLON;
        double rasterlrlat = ALL_ULLAT - (eachROWlat * (currentlowrow + 1));

        /**
         * Passing the parameters into the result and return
         */

        RasterResultParams.Builder result = new RasterResultParams.Builder();
        result.setRenderGrid(requiredPics);
        result.setDepth(currentDepth);
        result.setQuerySuccess(true);
        result.setRasterLrLat(rasterlrlat);
        result.setRasterLrLon(rasterlrlon);
        result.setRasterUlLat(rasterullat);
        result.setRasterUlLon(rasterullon);

        return result;
    }

    /**
     * Calculates the lonDPP of an image or query box
     * @param lrlon Lower right longitudinal value of the image or query box
     * @param ullon Upper left longitudinal value of the image or query box
     * @param width Width of the query box or image
     * @return lonDPP
     */
    private double lonDPP(double lrlon, double ullon, double width) {
        return (lrlon - ullon) / width;
    }
}
