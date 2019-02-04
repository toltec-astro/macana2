//Gtest Includes
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//Filter Include
#include "port_filter.h"

//Other Includes
#include <iostream>

namespace{

using namespace Eigen;
using namespace filter;

class FilterTest:public ::testing::Test
{
public:
    FilterTest(): generator(std::random_device{}()) {};
    ~FilterTest() override {}
    void TearDown() override {}
    void SetUp() override {}

    std::mt19937 generator;

    //Define some basic input parameters for all tests
    int nSamples = 1001; //number of samples in time series.
    int nTerms=32;
    int nCoef=2*nTerms+1;

    //Filter parameters
    double LowpassFilterKnee = 25.;  //Upper frequency cutoff
    double HighpassFilterKnee = 5.; //Lower frequency cutoff
    double samplerate = 64.; //Samplerate
    double nyquist = samplerate/2; //Nyquist Sampling rate

    double fHigh = LowpassFilterKnee/nyquist; //Scale upper frequency cutoff to Nyquist frequency
    double fLow = HighpassFilterKnee/nyquist; //Scale lower frequency cutoff to Nyquist frequency
    double aGibbs = 50.; //Gibbs parameter

    //Error tolerance for google test
    double err = 1e-5;

    //Parameters for random time series
    const double mean = 0.0;
    const double stddev = 0.01;

    std::vector<float> default_data = {-0.00146382, 0.0013453, -0.0187138, 0.0046065, -0.00214253, 0.00163712, -0.00827944, 0.00298595, 0.0105547, 0.000102154, 0.0117457, -0.00546841, -0.0104944, 0.00660682, -0.00625276, 0.0148596, -0.00829081, -0.0255912, -0.00888707, -0.00539781, 0.0101922, -0.00628956, -0.00482589, 0.00339587, -0.00121306, 0.0210886, -0.00371003, -0.00287389, -0.0230144, -0.0105935, -0.000615274, 0.0145502, 0.0135433, 0.00925328, -0.00243275, 0.0151561, 0.00197497, 0.0100886, 0.00439499, 0.00438945, 0.00645743, -0.00128149, -0.0168599, 0.0177643, -0.00613857, 0.00469861, -0.00582398, 0.00668493, -0.00103692, 0.00149386, 0.00624049, 0.0153727, 0.0117067, 0.0107825, -0.0205006, 0.0117196, -0.0145473, 0.00136395, -0.0111552, -0.0171463, 0.0112422, -0.0173985, -0.0147975, -0.0158694, 0.0148247, -0.00727862, 0.00754843, -0.001128, 0.00984235, 0.00326633, -0.0103745, -0.000764704, -0.0208402, 0.00389231, 0.00243215, 0.00455092, 0.00275194, 0.0291628, 0.00272422, -0.0320464, 0.0186225, -0.0209501, 0.0105544, 0.00310367, -1.22802e-05, 0.00404831, -0.0108115, 0.0141863, -0.00400148, 0.00926096, -0.00358203, 0.00126072, 0.00387892, -0.00569566, -0.00634654, 0.00882249, -0.00677104, 0.00204175, 0.0135715, -0.02453, -0.00315325, -0.00379922, -0.00608541, 0.0135717, 0.000195746, 0.0132359, -0.000912438, -0.00208138, -0.0161209, 0.00281664, 0.00785215, -0.00316253, 0.00353801, -0.00271609, -0.0177443, -0.000590157, -0.0153723, -0.000539041, 0.00386642, 0.00129153, 0.0154308, -0.00388037, 0.000763218, -0.00644921, -0.0168173, -0.00900313, -0.00383684, -0.00587587, -0.00130386, -0.0237398, 0.0101014, -0.00981124, 0.00465062, 0.0117503, -0.0154914, -0.00944311, -0.00524505, 0.0106121, 0.0152626, -0.0108715, -0.00395067, -0.018694, 0.00886654, -0.000245811, 0.00281015, -0.0043059, -0.00282224, 0.0098182, -0.00226851, 0.00680064, 0.00137044, -0.011258, -0.00171483, -0.000395065, 0.0151453, -0.00126506, 0.000306945, -0.00368253, 0.00983281, -0.00149726, -0.00584821, -0.00432586, 0.0141966, -0.00127846, -0.00704493, 0.0024827, 0.0181503, 0.01151, 0.0132445, -0.0084942, 0.0101566, 0.0116719, -0.000821409, -0.000890962, -0.000626403, -0.00568668, 0.00450628, -0.00503789, 0.00950093, -0.00502095, 0.0182362, -0.0093429, 0.00246156, -0.0033775, 0.00884862, -0.00787764, -0.00248331, -0.0038751, -0.00916308, -0.0128537, 0.00527725, -0.013623, -0.0124964, -0.00396654, -0.00314504, -0.0119639, -0.0140579, 0.00954706, -0.00564152, 0.00435944, -0.00293951, 0.000505313, 0.011321, -0.00730551, -0.00701741, -0.00393573, 0.00568603, -0.00441037, 4.47696e-06, -0.00113014, -0.00816183, -0.0023439, -0.00880084, 0.0133834, -0.00747356, 0.000238353, -0.00703211, 0.0175569, -0.0147934, -0.00592084, -0.00104689, 0.00193023, -0.00584238, 0.0235858, -0.0226491, 0.0283542, -0.0117708, -0.00770908, 0.000631497, 0.0256777, -0.00827822, 0.00763801, -0.0114972, -0.0117758, 0.00620899, 0.000476455, 0.0098074, -0.0123425, 0.0061932, -0.00922346, -0.00122258, 0.00685395, -0.00947542, 0.00379197, -0.0111819, -0.0258622, -0.0163714, 0.000379023, 0.0148602, 0.00814239, -0.0123476, -0.00409123, -0.00435955, 0.00702159, 0.00772662, 0.00221592, -0.0133679, 0.000629221, 0.0107924, -0.00467678, 0.00886887, -0.00579663, -0.0129847, -0.000285545, -0.00291079, 0.0034805, -0.0125291, 0.00492484, 0.00943752, -0.00268436, 0.00437742, 0.000593377, 0.00267865, -0.0100421, 0.0205674, 0.0167491, -0.00362788, -0.00726209, 0.00809637, -0.00875982, 0.00461384, -0.00449473, -0.00354654, 0.00722712, 0.00852314, 0.0117073, -0.00692884, 0.0213975, 0.0334928, -0.00239757, -0.00799365, -0.012033, -0.0052173, -0.00640274, 0.00655552, 0.0154968, 0.00677434, 0.00371682, -0.0158272, -0.00131305, 0.00224898, -0.00613299, -8.1768e-05, 0.0014908, 0.000753673, 0.00937905, 0.00630095, -0.00683983, 0.00765506, -0.00235396, 0.00897751, -0.00446957, -0.0127854, 0.0117484, 0.00110111, -0.00572371, 0.0156879, -0.000479286, -0.00458104, -0.0274776, -0.00515632, 0.0135409, 0.00197834, -0.00629265, 0.0222731, 0.00483006, -0.0119772, -0.0102637, 0.00127444, 0.017825, -0.0117528, -0.00774486, 0.0111866, 0.00823291, 0.00232714, -0.00540678, -0.00386004, 0.018626, 0.00193769, -0.0170498, -0.000494642, -0.0109847, -0.00948904, 0.00920589, -0.0252527, -0.00757891, -0.00567621, 0.00145231, -0.00021339, 0.00920078, -0.00668544, 0.00988638, -0.0150407, -0.00280452, 0.000589543, -0.0157093, 0.00871256, -0.0127608, -7.94979e-05, -0.00127517, -0.000522599, 0.00459375, -0.00687795, -0.0100312, -0.00610632, 0.00741354, -0.00377502, -0.0256207, -0.0147448, 0.00208709, 0.00990315, -0.00960484, 0.00267547, 0.015975, -0.00591776, 0.0106281, 0.00938553, 0.00226961, 0.0104078, 0.0177395, -0.00325351, -0.00736686, 0.00421624, 0.0205931, 0.00180534, -0.00373851, -0.013874, -0.00314101, -0.0108416, -0.0129715, 0.00947178, -0.00383016, 0.00472404, 0.00660181, 0.00603737, -0.00712546, -0.0054734, -0.0112765, 0.00385673, 0.00522128, -0.00527631, -0.0199117, 0.0121419, 0.00194943, -0.00353265, -0.00268178, -0.00252151, -0.026003, 0.0126732, -0.00698022, 0.00279553, 0.00270233, -0.00144387, -0.00488562, -0.00379344, 0.000924234, 0.0144168, 0.00706693, 0.00138135, 0.00383296, -0.00195424, 0.00350079, -0.0072443, -0.00354873, 0.00484002, -0.00346443, 0.0134539, 0.00246892, 0.00691835, 5.77599e-05, 0.00442391, -0.00922533, 0.00247923, 0.0007008, -0.000262607, 0.0149447, -0.000940468, 0.0101989, 0.00267996, 0.0073331, 0.00194967, -0.00904243, -0.0109351, 0.0123939, -0.0254097, -0.0108378, 0.00221882, 0.0120624, -0.00982211, 0.00388979, -0.000570549, -0.00444891, -0.00844491, 0.000615206, 0.0084769, 0.0071135, -0.00787109, -0.00605049, -0.0221345, -0.0185275, 0.0052713, -0.0098071, -0.00508354, -0.00951543, -0.00416532, -0.00584327, -0.00834383, -0.0047703, -0.00443108, -0.000861755, 0.0145326, -0.00290669, -0.00473628, -0.00263845, -0.00754708, 0.00639671, 0.0131621, 0.0037658, -0.000861914, 0.0260894, 0.00282825, -0.0129467, -0.00709605, 0.00850824, 0.0211541, 0.0119274, -0.0010508, -0.00475818, 0.0166539, -0.0169803, -0.00975323, 0.00286802, -0.028938, -0.0090465, 0.000431254, 0.00142143, -0.00746636, 0.0175214, 0.00742451, -0.0107145, 0.00090967, -0.000118416, 0.0121911, -0.00809848, 0.00507367, 0.007608, -0.00310818, 0.0107207, -0.0156409, -0.0120762, -0.00714422, 0.0155586, -0.00496367, 0.00187297, -0.0111635, -0.00445046, -0.0108473, -0.00326216, -0.000790771, -0.00360563, -0.00113692, -0.00913437, 0.008472, -0.000685937, 0.00918652, 0.00845642, 0.0125608, 0.00832128, -0.00650871, -0.00115916, 0.00985428, -0.00581525, 0.00946452, -0.00223969, -0.0047589, 0.00193619, -0.00586358, -0.00238768, -0.00529425, 0.0120506, -0.00265635, -0.00128535, 0.0183106, -0.000547503, 0.00833686, -0.00196416, -0.0062162, -0.00179572, 0.0100507, -0.00390093, -0.0109185, -0.00239214, 0.00747579, -0.0233939, -0.00112812, -0.00307341, 0.00311576, -0.00279058, -0.0186793, -0.00769162, -0.00813319, -0.0116624, 0.00984687, -0.0185186, -0.00706547, 0.0106825, -0.00491416, -0.0116783, 0.0138808, 0.00916554, 0.00192349, 0.00675454, 0.015904, 0.00657191, -0.00516508, 0.00161628, -0.0185241, -0.00322557, 0.00638492, 0.000740254, 0.000103586, -0.0111372, 0.00200773, 0.00400462, 0.00758657, 0.00256225, -0.0123283, 0.012767, 0.00870453, -0.0125194, 0.019174, -0.00953392, 0.0106227, -0.00589873, 0.013007, 0.00953267, -0.0012614, 0.0020385, 0.0143346, -0.0161178, 0.0152188, -0.000271384, 0.00921705, -0.00244183, -0.00811168, -0.00940522, -0.00683866, 0.00624366, 0.00518851, 0.0133499, -0.000127415, -0.0242106, 0.00088361, -0.000993983, -0.0166327, 0.000209465, 0.00733893, -0.00894768, 0.0189583, -0.00532601, 0.00266947, 0.00635124, 0.00521534, -0.00443554, -0.0027043, 0.0118315, -0.00645307, 0.00275187, -0.00597412, 0.00293763, -0.00685451, 0.00670444, -0.00758654, -0.00171357, 0.00016647, 0.00415241, -0.0171463, 0.00710353, 0.0108248, -0.00352332, -0.0126008, -0.00445409, -0.00425396, 0.00469941, -0.0095242, -0.010252, 0.00201316, 0.00696668, -0.00944373, -0.00740156, -0.00601917, -0.00249543, -0.00550116, 0.00136189, 0.0060288, -0.0205721, -0.00957341, 0.00298704, 0.00743144, 0.0127754, 0.01174, 0.00760148, 0.00298727, 0.017184, 0.0152179, -0.000617601, 0.000863676, 0.00430346, -0.00205344, -0.00678251, 0.0149416, -0.00120295, -0.00467559, -0.017191, 0.00290194, -0.0228182, -0.00942073, 0.00243156, -0.00354242, -0.000236125, -0.00744653, -0.00305193, 0.0204858, -0.026756, 0.00355898, 0.0118392, 0.00078338, 0.00860617, 0.00164093, 0.00157132, 0.00864563, 0.00688557, 0.0156244, 0.00575976, 0.00115453, 0.00774618, 0.00224118, -0.00849043, 0.00728544, 0.0143132, -0.0116179, -0.0100805, 0.00991093, 0.0189471, 0.0118983, 0.00566126, 0.000102677, 0.00826605, 0.0208867, -0.0160984, 0.0193512, 0.0140348, 0.0145061, -0.00481359, -0.00688219, 9.30049e-05, -0.0128774, 0.00113972, 0.00369326, 0.0195469, -0.00332844, -0.0204236, -0.00337809, -0.00974712, 0.0119946, -0.00892576, 0.00186458, -0.00506792, -0.00834328, -0.00223765, 0.0210094, -0.000558188, -0.00189972, -0.0145841, -0.00209296, -0.00907228, -0.000276518, -0.0100514, -0.00924995, -0.0136941, -0.0084146, -0.0117754, 0.00079418, -0.00756051, 0.00461642, 0.00282106, 0.00120929, 0.00528317, 0.00267308, -0.00166291, 0.00903335, -0.00127763, 0.00825044, -0.0195938, -0.0051208, 0.000704956, -0.00673837, 0.0119819, 0.00274125, 0.00826672, 0.00877626, -0.00393697, 0.0111674, 0.0125706, -0.000520402, -0.00250833, 0.00211367, 0.00692461, 0.00283706, 0.00263649, 0.0036395, -0.00192564, 0.0182662, -0.0103054, 0.00238505, -0.000141803, 0.00194606, 0.00480954, 0.0195272, -0.0169332, -0.00250419, 0.00578147, -0.0195273, 0.00257959, -0.0046732, 0.00320296, -0.0100432, -0.0113119, -0.0172939, -0.00877084, -0.0172505, -0.000990046, 0.0146654, -0.00256396, -0.00172531, -0.00239401, 0.022928, 0.0183907, 0.00237446, -0.00592667, 0.00394362, -0.00951821, -0.00175899, 0.00759519, -0.0100693, 0.0187191, 0.0047909, 0.0124127, -0.00514528, 0.0179953, 0.00777841, -0.00572023, -0.00252904, 0.00118107, -0.00301326, -0.00946566, -0.000242634, -0.00195974, 0.00183929, -0.0193146, -0.0163441, -0.015359, -0.0149805, 0.00279401, -0.0142756, -0.00858574, 0.00965335, -0.0150562, -0.0147738, -0.00377309, 0.0042173, 0.00680105, 0.0177316, 0.00751812, 0.00438632, -0.00483578, 0.00888007, -0.000566399, 0.00233661, 0.00843893, 0.0168605, -0.00590584, 0.00157813, -0.0154866, 0.00553573, -0.00125404, -0.00450803, -0.000505595, -0.00178169, 0.00236804, -0.0113701, 0.000440195, 0.00325204, 0.00271326, 0.0103224, 0.00496743, 0.00503979, -0.00276522, 0.00944774, 0.00534092, 0.00715707, 0.00927121, 0.00779704, 0.0120519, -0.013669, 0.0031565, 0.0100765, 0.0134123, 0.00508031, 0.0114199, 0.00878356, 0.00684186, -0.00415671, -0.00332741, 0.00559858, 0.00916494, 0.0207005, -0.00534523, 0.0163499, 0.00469594, -0.0263033, 0.0120134, 0.00915616, 0.0195757, 0.00508856, 0.0141581, 0.00397566, -0.00816497, -0.011804, -0.00798932, -0.012321, 0.00370647, 0.0118071, -0.0067009, -0.00284561, 0.00561738, -0.00832706, 0.0259991, -0.00494587, -0.00742179, 0.00608499, -0.00948318, -0.0103867, -0.0108092, 0.0053772, -0.00721695, 0.00554144, 0.0111569, 0.000407877, -0.0087944, 0.00410716, 0.00759595, -0.00112604, 0.00981577, 0.00361152, -0.0199203, -0.0129385, 0.00239944, -0.0137231, -0.00672274, -0.0153511, -0.0051009, -0.0044967, 0.0164954, -0.00856568, -0.0150982, -0.00120542, 0.0148974, -0.00130997, -0.00360708, 0.000741691, 0.00331897, 0.0119874, 0.010928, -0.00608949, -0.00592497, -0.00996312, -0.0148426, -0.000130615, 0.0176637, 0.00382988, -0.00666377, 0.0106181, -0.00597681, -0.00883804, -0.0174931, 0.00957817, -0.015444, 0.0110085, 0.0066106, 0.00297802, 0.0176334, 0.0134688, 0.00817577, 0.00269194, -0.00786617, -0.00989893, 0.00144086, -0.00226464, 0.0166988, 0.000457771, 0.004719, -0.000891262, 0.00121418, 0.00680879, 0.00839736, -0.00200556, 0.00800338, -0.00320243, -0.00414775, 0.00108123, 0.0051069, 0.0131504, -0.00114125, 0.00483941, -0.00942051, 0.00784224, -0.00762005, 0.00294447, 0.0175732, -0.0134622, 0.00384521, -0.0114729, -0.00138036, -0.0143263, -0.0180066, -0.00843455, 0.0213703, 0.00307682, 0.0144778, -0.0061029, 0.00940555, 0.0129604, -0.0100221, -0.00238803, -0.000562867, -0.00401814, -0.00804801, -0.00416178, 0.0158892, -0.0107325};
    std::vector<float> expected_from_data = {0.00286022,0.00278465,-0.00620474,0.00220469,0.00578905,0.00159886,0.00589,-0.00682952,-0.0170247,0.00218687,0.0125046,0.0042532,0.00070274,-0.00138685,-0.00391041,-0.00204679,-8.32066e-05,0.00305048,0.00243896,-0.00446194,-0.00330381,0.000506354,7.05338e-05,0.00144625,0.00276968,0.00705251,0.00109708,-0.0116981,-0.00314452,0.00354876,-0.00116168,0.0047067,0.00337385,-0.00367254,0.00190888,0.00070175,-0.00816647,1.69418e-05,0.00648138,-0.00525394,-0.00201503,0.00872059,0.00186598,-0.000565443,-0.000771272,-0.0087125,-0.00178476,0.0117406,-0.000362501,-0.0109,0.0071807,0.00474202,-0.0116495,-0.00174649,0.00548469,0.00834178,0.00275727,-0.0101508,-0.000303625,-0.00157376,-0.000595106,0.0126212,-0.00828609,-0.00666941,0.014798,-0.0068559,-0.0135208,0.00789321,0.00522938,-0.00657545,0.00238975,0.0110654,0.000929619,-0.00222854,-0.00360054,-0.0117893,-0.00322393,0.00919565,0.00710342,-0.00272253,-0.00486038,-0.000675524,0.00284036,0.00913454,-0.00445841,-0.0112064,0.00935569,0.00179865,-0.00693228,0.00438675,-0.000602691,-0.00381575,-0.00257813,0.00451513,0.00687626,-0.00738832,0.00147336,0.00442192,-0.0096708,0.00261504,0.00840476,0.00128538,0.000910657,-0.00761609,-0.0116988,-0.00213822,0.00453292,0.00831038,0.0162889,0.000980624,-0.0186958,0.00266527,0.00368688,-0.0148106,0.00426427,0.00486963,-0.00381265,0.0105151,-0.000396631,-0.0072229,0.00732928,0.000333097,-0.00879771,-0.000425218,0.00508376,0.000102216,-0.00256324,0.00114423,-0.000185951,-0.00052243,0.00396974,-0.00173957,-0.00524303,0.00565122,0.000709629,-0.0122048,0.00523812,0.0154731,-0.00336502,-0.00785471,-0.00292551,0.000572705,0.00661763,-0.00467169,-0.00874802,0.00744812,0.00795179,-0.0030047,-0.00554552,0.000354988,0.00218019,-0.00135132,0.00865221,0.00684593,-0.0145491,-0.0113743,0.00297361,0.00375051,0.00561742,0.00556751,-0.00332799,-0.00579652,0.00660902,0.00632452,-0.00856337,-0.00507553,0.00104453,-0.00526587,-0.00247734,0.0118497,0.0110106,-0.0076629,-0.00248139,0.00376649,-0.0126954,-0.00320066,0.00694567,0.00281292,0.00432152,-0.00793171,0.00527986,0.0129395,-0.0186669,-0.00459775,0.012911,-0.00357365,0.00191267,-0.00158905,-0.00721347,0.00205921,0.000877712,0.00631462,0.00242457,-0.00619135,0.00507967,-0.000650007,-0.0109415,0.00698367,0.011884,-0.0122303,-0.0144351,0.00745242,0.0112868,0.00606214,-0.00276314,-0.0113153,0.004024,0.00390602,-0.0144236,0.00179122,0.0175629,0.0038637,-0.00525239,-0.00667275,-0.00567648,-0.00504988,-0.00509408,0.0114569,0.0193726,-0.00139646,-0.0126235,-0.00701066,0.00158036,0.00712869,-0.000305182,-0.00138474,-0.00110517,-0.011244,0.000410726,0.0162908,0.00487172,-0.00689273,-0.00571836,-0.00276723,0.0015854,0.00529384,0.00167047,-0.00125492,-0.000855849,-0.00358913,-0.000254664,0.000930109,-0.00204998,0.00270413,0.000862272,-0.00113603,0.00135655,-0.00165187,-3.07174e-05,0.00221315,0.00672662,0.00283933,-0.0122665,-0.00489275,0.0008916,-0.00279993,0.0061657,0.00343309,0.00216115,0.0026164,-0.00785506,0.00216531,0.00860348,-0.00621448,-0.00801766,0.000500587,0.00231816,0.000924814,0.00199565,-0.00200698,0.00117104,0.00947427,-0.00598398,-0.00729458,0.00377424,-0.0069982,0.00796112,0.0133048,-0.0151879,-0.00284127,0.00565108,-0.0117709,0.00200525,0.00974064,0.00502714,0.00617831,-0.00587502,-0.00471282,-0.00194183,-0.0117039,0.0033936,0.0127482,-0.00255671,-0.00916799,-0.00501316,0.0130086,0.0166177,-0.00658632,-0.00571289,-0.000831233,-0.0103913,-0.000759566,0.00137978,-0.00503338,0.00651936,0.0094443,-0.00179521,-0.00424455,0.00685259,0.00538348,-0.0108598,-0.00524473,0.00277108,-0.00374744,-0.00180633,0.00195286,0.00950671,0.00795975,-0.0075341,-0.00566091,-0.00549245,-0.00178637,0.0112537,-0.00311349,-0.00041435,0.0152364,-0.00813408,-0.0150499,-0.000333023,-0.000378324,0.00608541,0.0102634,0.00418588,-0.00455804,-0.00870299,1.24602e-05,-0.00307138,-0.00576682,0.00935378,0.0091219,-0.00050735,-0.00525611,-0.00755676,0.00214415,0.00488128,-0.00232034,0.0008145,0.00438704,-0.000330055,-0.00614782,-0.00310582,0.0029336,0.000715882,0.003504,0.00436822,-0.0052957,-0.00418627,0.000334997,0.000687956,0.00237301,0.000932453,0.00650659,0.0020606,-0.014888,-0.00639231,0.00705476,0.00412222,0.00142972,0.00172012,0.00540259,-0.00113291,-0.00925072,-0.00599229,-0.00332289,0.0111888,0.0100933,-0.00518515,0.00916131,-0.00311501,-0.0276728,-0.00259906,0.0112791,0.00537286,0.00746727,0.00671414,0.00582554,-0.00780674,-0.00996723,-0.00317704,-0.0106641,0.00775968,0.0133453,-0.00815723,-5.51096e-06,-0.00217476,-0.00343179,0.0169972,-2.39371e-05,-0.00999319,0.00800622,-0.00284373,-0.0118812,-0.000133165,0.00276141,-0.000523712,0.00350515,0.0105223,-0.00163146,-0.0135891,0.00134548,0.00417334,-0.00304259,0.0107837,0.00576601,-0.0152854,-0.00478363,0.0079697,-0.000726135,-0.00274398,-0.00124791,-0.00385611,0.0065423,0.0152217,-0.0029227,-0.0179829,-0.0004179,0.0113693,-0.00287739,-0.00690386,-0.000866908,0.00107401,0.011226,0.0100068,-0.00590715,-0.00629348,-0.00849338,-0.0153882,0.00256601,0.0181885,0.00589094,-0.00207832,0.00899202,0.00385843,-0.015293,-0.00619969,0.000697583,-0.00915788,0.00314803,0.00484331,-0.00275195,0.0109174,0.00895241,0.00169253,-0.000375777,-0.0124553,-0.00968111,-0.00328625,0.00127125,0.0105508,0.00381499,-0.00150555,-0.00115454,-0.00374933,0.00403989,0.00359239,-0.00313393,0.00216029,-0.00181857,-0.00893087,0.000158251,0.00524802,0.00523343,0.00408328,-0.00734223,-0.00442193,0.000443607,-0.00804233,0.0065198,0.0129908,-0.00179945,0.000894844,0.000660398,-0.000656049,-0.00558022,-0.0186641,-0.0016852,0.0141132,0.00855721,0.0033406,-0.00506973,0.00263024,0.0077753,-0.00892541,-0.0084848,0.002968,-0.00272852,-0.00861464,0.00576033,0.0128693,-0.000638148,0.000581944,0.00268015,-0.00917887,-0.00212499,0.00743256,-0.00451003,-0.0076924,0.00317496,0.00272154,0.00322773,0.00565772,-0.00614292,-0.00646751,0.00179053,0.0051531,0.0130295,0.000263928,-0.0189615,-0.00893688,0.00577628,0.00622989,-0.000249513,-0.00393661,-0.000313094,0.00710614,0.0126662,0.00652634,-0.00826817,-0.0171279,-0.0120445,0.000220723,0.0109514,0.0123573,-0.000900739,-0.00446984,0.00166856,-0.00210536,-0.000139044,-0.000460939,-0.00093081,0.00628839,-0.0061327,-0.00873447,0.00978835,0.00568152,-0.00436939,0.000111186,5.31335e-05,-0.00497725,-0.00394341,-0.000819992,0.00632089,0.0120342,-0.00261374,-0.00959219,0.000960581,0.000298703,0.00172875,-0.00142196,-0.00438519,0.0123857,0.00072528,-0.0195006,-0.000781329,0.00857556,0.00146825,0.00242454,0.00267127,0.00756778,0.00328964,-0.00769094,-0.00576157,-0.0124985,-0.00603643,0.0121456,-0.00452403,0.00530403,0.0295669,-0.00710598,-0.0199645,0.00164708,-0.00601542,-0.00116699,0.00877986,0.000928065,-0.00276843,-0.0019239,0.00251519,0.00283038,0.00118285,-0.00233773,-0.00221691,0.00412274,-0.00755585,-0.00350897,0.00917246,-0.00435015,0.00781164,0.0116068,-0.0152536,-0.00694459,0.000617937,-0.00530274,0.00749643,0.00678864,0.00160363,0.00551487,-0.00876516,-0.0147012,0.00520948,0.00760901,0.00225346,0.00844083,-0.00171354,-0.00757034,-0.00358843,-0.0078224,-0.00128684,0.00292922,0.00551851,0.0102651,0.00157306,0.000459208,-0.00277739,-0.0102125,-0.00353722,0.00234535,0.00640173,-0.00206795,-0.00733316,0.00536374,0.00106039,0.00395174,0.0114451,-0.00997331,-0.015318,-8.64166e-05,0.01005,0.0131323,-0.00178099,-0.012375,-0.00599874,0.00149322,0.00702495,0.00186057,-0.0054476,0.00113595,0.00460855,0.000898769,0.00350559,-0.000197182,-0.0103361,-0.00738171,0.00322508,0.00854834,0.00292899,-0.00483708,0.000162957,0.00650923,0.00148749,-0.00843336,-0.00313509,0.0104813,-0.00234742,-0.0139906,5.48738e-05,0.00744285,0.00894868,0.00149105,-0.011047,0.00187875,0.0088135,-0.00369398,-0.00281172,-0.00125395,-0.00408792,0.000553435,0.00123237,-0.000264463,0.00378789,0.00562778,-0.00173914,-0.0074285,0.00131477,0.00506656,-0.00397311,-0.00101873,0.00309613,-0.00554533,-0.00119676,0.00930024,-0.000249727,-0.00712293,0.00541588,0.00502639,-0.0100984,-0.00443676,0.00587612,0.00137455,0.000577007,-0.00146427,0.000539317,0.00843513,-0.00337244,-0.0105959,0.00171668,0.00412071,0.000634751,0.00145127,0.000908197,-0.00297865,-0.00528226,0.000235275,0.00628224,0.00312408,-0.00695675,0.00161592,0.0132029,-0.00875438,-0.00984509,0.00580732,-0.00686196,0.00478917,0.00837527,-0.00971024,0.0108012,-3.11226e-05,-0.0227416,0.00988728,0.0149783,-0.00120926,0.00184821,-0.00838767,-0.0110177,0.00259599,0.00786581,0.00131838,-0.00283735,0.00029542,-0.00345853,0.00160935,0.00808673,0.000512598,0.00531854,-0.00102363,-0.0202955,-0.0106402,0.00756181,0.0173863,0.011129,-0.00959964,-0.00788238,-0.000631932,-7.77056e-05,0.00992199,-0.00204725,-0.0150048,0.00253346,0.00650581,0.00125097,0.00663098,-0.00131447,-0.00907623,-0.000775622,0.00574182,-0.000170674,-0.00597771,0.00264562,0.00791879,-0.00196036,-0.00440448,0.00171127,-0.00724342,-0.0107811,0.0139537,0.0162367,-0.0079172,-0.00400678,0.0032633,-0.00304866,-0.000576937,-0.00294967,-0.00669996,3.20055e-06,0.00716631,-0.000695319,-0.010467,0.0122721,0.0253133,-0.00380572,-0.0174535,-0.00914499,-0.00992776,-0.00509256,0.00849105,0.0135666,0.0116236,0.00100417,-0.0124001,-0.00300819,0.00347384,-0.00720343,-0.000307793,0.000922838,-0.00169521,0.0102998,0.000786195,-0.00488825,0.00126359,-0.00180244,0.00547846,-0.00613276,-0.0126162,0.0107127,0.00201143,-0.00279168,0.0145385,0.00606755,-0.0077634,-0.020737,-0.0088687,0.0178878,-0.00134906,-0.00444847,0.0199449,0.00490538,-0.0125148,-0.0118161,0.00258804,0.0145903,-0.0101241,-0.0120592,0.0111077,0.00423452,3.28844e-05,-0.00737791,-0.00644031,0.0198504,0.00241498,-0.0131138,0.00534678,-0.00627601,0.00087079,0.0125521,-0.0137733,-0.00708952,0.0043287,-0.0017689,0.00858934,0.00305709,0.00174182,0.00483202,-0.00902124,-0.00135371,0.00086287,-0.00649003,0.00390669,-0.00131175,-0.00408885,0.00513136,0.00170684,0.00478505,0.0010295,-0.0102222,0.00230134,0.0127952,0.00113096,-0.0152705,-0.0130752,0.0109277,0.0106761,-0.00840327,0.00316055,0.00877111,-0.0076221,-0.000754868,0.00371272,-0.00827072,0.00225903,0.0099843,-0.00901421,-0.013428,0.00509391,0.0155799,0.00853893,-0.00584142,-0.00677525,-0.0002558,-0.00828383,-0.00634373,0.00742181,0.00247762,0.00138746,0.0101016,0.00364372,-0.00494709,-0.00721662,-0.00765484,0.00226616,0.0107722,-0.00611505,-0.0134684,0.0120012,0.00958208,-0.00480709,0.00751282,-0.00619951,-0.0141922,0.00803354,0.00254191,-0.000477835,0.00687706,-0.00332605,-0.00532895,-0.00611826,-0.00134002,0.0112198,0.00425949,-0.000777882,0.000214475,-0.00189873,-0.000265079,-0.00687999,-0.00514764,0.00226938,-0.00118109,0.00690306,0.00761068,-0.0014252,0.00457275,-0.00235735,-0.00907235,-0.000865351,-0.00486596,-0.000855866,0.00598502,-0.000777592,0.00224896,0.00302964,0.00326945,0.00471005,-0.0101729,-0.00285261,0.0111374,-0.0125748,-0.0135747,0.014584,0.00919777,-0.00328854,0.00216179,9.56674e-05,-0.00575871,-0.0083925,-0.00113819,0.0126204,0.00682732,-2.42333e-05,-0.00106807,-0.0144524,-0.00708782,0.0106522,0.00415849,-0.000571456,0.00131968,0.00105567,-0.000159461,-0.00375073,-0.00183692,-0.00311476,0.00203973,0.0123831,-0.000434066,-0.00765361,-0.00287727,-0.00885545,0.00229952,0.0132253,-0.0035558,-0.00131682,0.0168981,0.000187589,-0.0222219,-0.0126562,0.00210841,0.0131762,0.0113639,-0.0087492,0.000682191,0.0137851,-0.00917638,-0.00435173,0.00812345,-0.0165111,-0.00877224,0.0136626,-0.00168149,-0.000729874,0.01494,0.00436463,-0.0100944,-0.00890951};
    std::vector<float> expected_from_tensor = {0.0123573,-0.000900732,-0.00446983,0.00166857,-0.00210536,-0.000139052,-0.00046094,-0.000930808,0.00628839,-0.0061327,-0.00873447,0.00978834,0.00568152,-0.00436939,0.000111183,5.31452e-05,-0.00497724,-0.00394342,-0.000820003,0.00632088,0.0120342,-0.00261372,-0.00959219,0.000960577,0.000298702,0.00172874,-0.00142196,-0.00438521,0.0123857,0.000725302,-0.0195006,-0.000781346,0.00857557,0.00146825,0.00242453,0.00267127,0.00756779,0.00328966,-0.00769094,-0.00576157,-0.0124985,-0.00603646,0.0121456,-0.00452402,0.00530402,0.0295669,-0.00710596,-0.0199645,0.00164707,-0.00601541,-0.00116699,0.00877986,0.000928083,-0.00276843,-0.00192391,0.00251518,0.00283038,0.00118286,-0.00233773,-0.00221692,0.00412275,-0.00755584,-0.00350898,0.00917246,-0.00435015,0.00781163,0.0116068,-0.0152536,-0.0069446,0.00061793,-0.00530274,0.00749642,0.00678864,0.00160364,0.00551488,-0.00876514,-0.0147013,0.00520946,0.00760901,0.00225347,0.00844084,-0.00171353,-0.00757034,-0.00358843,-0.00782241,-0.00128685,0.00292921,0.0055185,0.0102651,0.00157307,0.000459209,-0.00277738,-0.0102125,-0.00353723,0.00234535,0.00640174,-0.00206793,-0.00733316,0.00536373,0.00106039,0.00395174,0.0114451,-0.00997329,-0.015318,-8.64359e-05,0.01005,0.0131323,-0.00178097,-0.012375,-0.00599875,0.00149322,0.00702495,0.00186058,-0.0054476,0.00113594,0.00460855,0.000898766,0.00350559,-0.000197168,-0.0103361,-0.00738172,0.00322507,0.00854834,0.002929,-0.00483708,0.000162945,0.00650923,0.00148751,-0.00843335,-0.00313511,0.0104813,-0.0023474,-0.0139906,5.48556e-05,0.00744284,0.00894868,0.00149107,-0.011047,0.00187874,0.0088135,-0.00369397,-0.00281173,-0.00125394,-0.00408792,0.000553427,0.00123237,-0.000264461,0.00378789,0.00562778,-0.00173913,-0.0074285,0.00131475,0.00506657,-0.0039731,-0.00101874,0.00309613,-0.00554533,-0.00119677,0.00930024,-0.000249715,-0.00712294,0.00541588,0.00502641,-0.0100984,-0.00443678,0.00587612,0.00137456,0.000577006,-0.00146427,0.000539315,0.00843513,-0.00337243,-0.0105959,0.00171667,0.00412071,0.000634753,0.00145127,0.000908196,-0.00297865,-0.00528226,0.00023526,0.00628224,0.00312409,-0.00695674,0.00161591,0.0132029,-0.00875437,-0.00984509,0.00580732,-0.00686196,0.00478915,0.00837528,-0.00971024,0.0108012,-3.10994e-05,-0.0227416,0.00988724,0.0149783,-0.00120925,0.00184821,-0.00838766,-0.0110177,0.00259598,0.00786581,0.00131839,-0.00283735,0.000295424,-0.00345853,0.00160935,0.00808673,0.000512598,0.00531855,-0.00102361,-0.0202955,-0.0106403,0.00756179,0.0173863,0.011129,-0.00959962,-0.00788238,-0.000631935,-7.77061e-05,0.00992199,-0.00204723,-0.0150048,0.00253344,0.00650581,0.00125098,0.00663098,-0.00131446,-0.00907622,-0.000775638,0.00574182,-0.00017066,-0.00597772,0.00264562,0.00791879,-0.00196035,-0.00440448,0.00171127,-0.00724341,-0.0107811,0.0139537,0.0162367,-0.00791719,-0.00400679,0.0032633,-0.00304865,-0.00057694,-0.00294966,-0.00669996,3.18247e-06,0.0071663,-0.000695308,-0.010467,0.012272,0.0253133,-0.00380568,-0.0174535,-0.009145,-0.00992776,-0.00509258,0.00849104,0.0135666,0.0116236,0.0010042,-0.0124001,-0.0030082,0.00347385,-0.00720343,-0.000307801,0.000922835,-0.00169522,0.0102998,0.000786211,-0.00488824,0.00126359,-0.00180244,0.00547847,-0.00613276,-0.0126162,0.0107127,0.00201144,-0.00279168,0.0145385,0.00606757,-0.00776339,-0.020737,-0.00886872,0.0178878,-0.00134905,-0.00444849,0.0199449,0.00490541,-0.0125148,-0.0118161,0.00258802,0.0145903,-0.0101241,-0.0120592,0.0111077,0.00423453,3.28921e-05,-0.00737791,-0.00644033,0.0198504,0.00241501,-0.0131138,0.00534677,-0.00627601,0.000870783,0.0125522,-0.0137732,-0.00708953,0.0043287,-0.0017689,0.00858934,0.00305709,0.00174181,0.00483203,-0.00902124,-0.00135372,0.000862881,-0.00649003,0.00390669,-0.00131175,-0.00408885,0.00513134,0.00170684,0.00478506,0.00102951,-0.0102222,0.00230133,0.0127952,0.00113098,-0.0152705,-0.0130752,0.0109277,0.0106761,-0.00840326,0.00316053,0.00877112,-0.00762209,-0.000754883,0.00371272,-0.00827072,0.00225901,0.00998431,-0.00901419,-0.013428,0.00509389,0.0155799,0.00853895,-0.00584141,-0.00677525,-0.000255792,-0.00828383,-0.00634375,0.00742181,0.00247761,0.00138746,0.0101016,0.00364373,-0.00494708,-0.00721662,-0.00765484,0.00226615,0.0107722,-0.00611504,-0.0134684,0.0120012,0.00958209,-0.00480708,0.00751282,-0.00619949,-0.0141922,0.00803353,0.00254193,-0.000477837,0.00687707,-0.00332604,-0.00532895,-0.00611826,-0.00134002,0.0112198,0.0042595,-0.000777885,0.000214485,-0.00189873,-0.000265081,-0.00687999,-0.00514766,0.00226938,-0.00118109,0.00690305,0.00761069,-0.00142519,0.00457275,-0.00235734,-0.00907235,-0.000865352,-0.00486596,-0.00085588,0.00598502,-0.000777591,0.00224896,0.00302964,0.00326945,0.00471006,-0.0101728,-0.00285262,0.0111375,-0.0125748,-0.0135747,0.014584,0.00919779,-0.00328854,0.00216179,9.5678e-05,-0.0057587,-0.00839251,-0.00113821,0.0126204,0.00682733,-2.42232e-05,-0.00106806,-0.0144523,-0.00708784,0.0106522,0.0041585,-0.000571462,0.00131968,0.00105567,-0.000159464,-0.00375073,-0.00183691,-0.00311476,0.00203972,0.0123831,-0.000434048,-0.00765361,-0.00287727,-0.00885546,0.0022995,0.0132253,-0.00355579,-0.00131683,0.0168981,0.000187621,-0.0222219,-0.0126563,0.00210839,0.0131762,0.0113639,-0.00874919,0.000682171,0.0137851,-0.00917636,-0.00435174,0.00812346,-0.0165111,-0.00877227,0.0136626,-0.00168147,-0.000729887,0.01494,0.00436466,-0.0100944,-0.00890953,0.00286021,0.00278466,-0.00620475,0.00220468,0.00578905,0.00159885,0.00589,-0.00682949,-0.0170247,0.00218685,0.0125046,0.00425321,0.000702742,-0.00138684,-0.0039104,-0.00204679,-8.32081e-05,0.00305048,0.00243896,-0.00446194,-0.00330381,0.000506345,7.05314e-05,0.00144626,0.00276968,0.00705251,0.00109709,-0.0116981,-0.00314454,0.00354875,-0.00116168,0.0047067,0.00337386,-0.00367254,0.00190888,0.000701757,-0.00816647,1.69314e-05,0.00648138,-0.00525394,-0.00201504,0.00872059,0.00186599,-0.000565432,-0.000771267,-0.0087125,-0.00178477,0.0117406,-0.000362482,-0.0109,0.00718069,0.00474204,-0.0116495,-0.0017465,0.00548468,0.00834178,0.00275728,-0.0101508,-0.000303626,-0.00157376,-0.000595125,0.0126212,-0.00828607,-0.00666942,0.014798,-0.00685589,-0.0135208,0.0078932,0.00522939,-0.00657546,0.00238974,0.0110654,0.000929639,-0.00222854,-0.00360053,-0.0117893,-0.00322395,0.00919564,0.00710342,-0.00272253,-0.00486037,-0.000675529,0.00284035,0.00913454,-0.00445839,-0.0112064,0.00935568,0.00179866,-0.00693228,0.00438675,-0.000602676,-0.00381574,-0.00257814,0.00451513,0.00687627,-0.00738832,0.00147335,0.00442193,-0.0096708,0.00261502,0.00840476,0.00128539,0.000910672,-0.00761608,-0.0116989,-0.00213824,0.00453292,0.00831037,0.0162889,0.000980648,-0.0186958,0.00266525,0.0036869,-0.0148106,0.00426425,0.00486964,-0.00381267,0.0105151,-0.000396611,-0.00722291,0.00732927,0.000333115,-0.00879771,-0.000425234,0.00508376,0.000102225,-0.00256324,0.00114422,-0.000185956,-0.000522433,0.00396975,-0.00173956,-0.00524304,0.00565122,0.000709637,-0.0122048,0.0052381,0.0154731,-0.003365,-0.00785471,-0.00292552,0.000572699,0.00661763,-0.00467168,-0.00874803,0.00744811,0.0079518,-0.00300469,-0.00554553,0.000354985,0.00218019,-0.00135132,0.00865219,0.00684595,-0.0145491,-0.0113743,0.0029736,0.00375052,0.00561742,0.00556752,-0.00332798,-0.00579653,0.00660901,0.00632453,-0.00856336,-0.00507554,0.00104454,-0.00526586,-0.00247736,0.0118497,0.0110107,-0.00766289,-0.0024814,0.0037665,-0.0126954,-0.00320067,0.00694567,0.00281292,0.00432152,-0.00793171,0.00527985,0.0129395,-0.0186669,-0.00459777,0.012911,-0.00357364,0.00191267,-0.00158905,-0.00721348,0.00205921,0.00087771,0.00631462,0.00242458,-0.00619136,0.00507967,-0.000649991,-0.0109415,0.00698365,0.0118841,-0.0122303,-0.0144351,0.00745239,0.0112868,0.00606215,-0.00276314,-0.0113153,0.004024,0.00390603,-0.0144236,0.00179119,0.0175629,0.00386372,-0.00525237,-0.00667274,-0.00567648,-0.00504989,-0.0050941,0.0114569,0.0193726,-0.00139645,-0.0126235,-0.00701067,0.00158034,0.00712869,-0.000305167,-0.00138474,-0.00110517,-0.011244,0.000410702,0.0162908,0.00487174,-0.00689272,-0.00571836,-0.00276724,0.00158539,0.00529384,0.00167048,-0.00125491,-0.000855851,-0.00358914,-0.000254668,0.000930111,-0.00204998,0.00270413,0.000862278,-0.00113603,0.00135655,-0.00165188,-3.07183e-05,0.00221315,0.00672662,0.00283935,-0.0122665,-0.00489276,0.000891596,-0.00279993,0.0061657,0.00343308,0.00216115,0.00261641,-0.00785506,0.0021653,0.00860349,-0.00621447,-0.00801768,0.00050058,0.00231816,0.000924808,0.00199566,-0.00200697,0.00117104,0.00947426,-0.00598397,-0.00729459,0.00377423,-0.0069982,0.00796111,0.0133048,-0.0151879,-0.00284129,0.00565109,-0.0117709,0.00200523,0.00974063,0.00502714,0.00617833,-0.005875,-0.00471282,-0.00194182,-0.0117039,0.00339359,0.0127482,-0.0025567,-0.00916799,-0.00501318,0.0130086,0.0166177,-0.0065863,-0.0057129,-0.000831232,-0.0103913,-0.000759582,0.00137978,-0.00503338,0.00651934,0.0094443,-0.0017952,-0.00424455,0.00685259,0.00538349,-0.0108598,-0.00524473,0.00277107,-0.00374744,-0.00180633,0.00195285,0.00950671,0.00795977,-0.00753409,-0.00566091,-0.00549245,-0.00178639,0.0112537,-0.00311348,-0.00041436,0.0152364,-0.00813406,-0.0150499,-0.000333037,-0.000378329,0.0060854,0.0102634,0.00418589,-0.00455803,-0.008703,1.24532e-05,-0.00307138,-0.00576683,0.00935377,0.00912191,-0.000507341,-0.0052561,-0.00755676,0.00214414,0.00488127,-0.00232033,0.000814495,0.00438704,-0.000330043,-0.00614782,-0.00310583,0.0029336,0.000715876,0.003504,0.00436823,-0.00529569,-0.00418627,0.000334989,0.000687955,0.00237301,0.000932452,0.00650659,0.00206062,-0.014888,-0.00639234,0.00705474,0.00412222,0.00142973,0.00172013,0.0054026,-0.0011329,-0.00925071,-0.00599229,-0.00332291,0.0111888,0.0100933,-0.00518514,0.00916132,-0.00311497,-0.0276728,-0.00259909,0.0112791,0.00537287,0.00746727,0.00671415,0.00582556,-0.00780672,-0.00996723,-0.00317705,-0.0106641,0.00775966,0.0133453,-0.00815721,-5.50923e-06,-0.00217476,-0.00343181,0.0169972,-2.39156e-05,-0.00999319,0.00800622,-0.00284372,-0.0118812,-0.000133178,0.0027614,-0.000523716,0.00350514,0.0105223,-0.00163144,-0.0135891,0.00134547,0.00417334,-0.0030426,0.0107837,0.00576604,-0.0152854,-0.00478365,0.0079697,-0.000726129,-0.00274398,-0.00124791,-0.00385612,0.00654229,0.0152217,-0.00292267,-0.0179829,-0.000417918,0.0113693,-0.00287739,-0.00690386,-0.000866913,0.001074,0.011226,0.0100069,-0.00590713,-0.00629348,-0.00849337,-0.0153882,0.00256598,0.0181885,0.00589096,-0.00207832,0.00899202,0.00385846,-0.015293,-0.0061997,0.00069758,-0.00915789,0.00314801,0.00484331,-0.00275196,0.0109174,0.00895243,0.00169254,-0.000375766,-0.0124553,-0.00968112,-0.00328626,0.00127124,0.0105508,0.003815,-0.00150554,-0.00115455,-0.00374934,0.00403989,0.0035924,-0.00313393,0.00216029,-0.00181857};

};

/*----------------------------------------------------------------------------------------------*/

using namespace testing;

MATCHER_P(NearWithPrecision, precision, "") {
    return abs(get<0>(arg) - get<1>(arg)) < precision;
}

/*----------------------------------------------------------------------------------------------*/

TEST_F(FilterTest,MacanaFilter){
    //Setup Vectors
    VecDoub hValues(nSamples,0.);
    VecDoub storage(nSamples,0.);
    VecDoub digitalFilterTerms(nCoef,1.);

    for(int i = 0; i<nSamples; i++) hValues[i] = default_data[i];

    //Get fake Data
    //MakeData<VecDoub>(hValues, nSamples, mean, stddev);

    //Make Filter
    digiFilter(fLow, fHigh, aGibbs, nTerms, digitalFilterTerms);

    //Macana Convolution
    MacanaFilter(hValues, digitalFilterTerms, storage, nSamples, nTerms, nCoef);

    std::vector<double> c(&hValues[0] + nTerms,&hValues[0] + nSamples - nTerms);

    EXPECT_THAT(c, Pointwise(NearWithPrecision(err), expected_from_tensor));

}

/*----------------------------------------------------------------------------------------------*/

TEST_F(FilterTest,EigenFFTFilter){
    //Setup Vectors
    Eigen::VectorXf digitalFilterTerms(nCoef);
    Eigen::VectorXf convolution(nSamples);

    Eigen::VectorXf hValues = Eigen::VectorXf::Map(default_data.data(), nSamples);

    //Get fake Data
    //Eigen::VectorXf hValues(nSamples);
    //hValues.setZero();
    //MakeData<VectorXf>(hValues, nSamples, mean, stddev);

    EigenDigitalFilter(fLow, fHigh, aGibbs, nTerms, digitalFilterTerms); //Make Filter

    //Zero pad the filter to match size in order to do FFT
    Eigen::VectorXf digitalFilterTerms_scaled(nSamples);
    digitalFilterTerms_scaled.setZero();
    digitalFilterTerms_scaled.segment((nSamples-1)/2.-nCoef/2,nCoef) = digitalFilterTerms;

    EigenFFTFilter(hValues, digitalFilterTerms_scaled, convolution);

    std::vector<double> c(convolution.data(),convolution.data() + nSamples);

    EXPECT_THAT(c, Pointwise(NearWithPrecision(err), expected_from_data));
}

/*----------------------------------------------------------------------------------------------*/

TEST_F(FilterTest,EigenTensorFilter){
    //Setup Vectors
    Eigen::Tensor<float, 1, ColMajor> convolution(static_cast<Index>(nSamples - nCoef + 1));
    Eigen::VectorXf digitalFilterTerms(nCoef);

    //hValues.setZero();
    Eigen::VectorXf hValues = Eigen::VectorXf::Map(default_data.data(), nSamples);
    Eigen::Tensor<float, 1> hValues_tensor = Vector_to_Tensor<Eigen::VectorXf>(hValues, nSamples);

    //Get fake Data
    //Eigen::Tensor<float, 1, ColMajor> hValues_tensor(static_cast<Index>(nSamples));
    //MakeData<Eigen::Tensor<float, 1, ColMajor>>(hValues_tensor, nSamples, mean, stddev);

    //Make Filter
    EigenDigitalFilter(fLow, fHigh, aGibbs, nTerms, digitalFilterTerms);

    //Convert Eigen::VectorXf to Eigen::tensor
    Eigen::Tensor<float, 1> digFiltT_tensor = Vector_to_Tensor<Eigen::VectorXf>(digitalFilterTerms, nCoef);

    //Vector to tell Eigen which dimension to run convolution on.
    Eigen::array<ptrdiff_t, 1> dims{0};

    //Eigen Tensor Convolution
    EigenTensorFilter(hValues_tensor,digFiltT_tensor,dims,convolution);

    std::vector<double> c(convolution.data(),convolution.data() + nSamples - nCoef + 1);

    EXPECT_THAT(c, Pointwise(NearWithPrecision(err), expected_from_tensor));
}

}//namespace
